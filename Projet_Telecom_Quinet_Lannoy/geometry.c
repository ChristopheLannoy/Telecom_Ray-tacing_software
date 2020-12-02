#include "geometry.h"
#include <math.h>
#include <stdio.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y)) // Returns the max between x and y
#define MIN(x, y) (((x) < (y)) ? (x) : (y)) // Returns the min between x and y

Vector newVec(double x, double y) {
    Vector vec = {x,y};
    return vec;
};

//Adds the Vector v2 to v1 (v1 + v2)
void vecAdd(Vector *v1, Vector v2) {
    v1->x += v2.x;
    v1->y += v2.y;
}

//Substracts the Vector v2 to v1 (v1 - v2)
void vecSub(Vector *v1, Vector v2) {
    v1->x -= v2.x;
    v1->y -= v2.y;
}

//Returns a new Vector which is the substraction of v1 and v2 (v1 - v2)
Vector newVecSub(Vector v1, Vector v2) {
    Vector sub = {v1.x - v2.x, v1.y - v2.y};
    return sub;
}

//Returns the norm of the Vector v
double norm(Vector v) {
    return sqrt(pow(v.x,2)+pow(v.y,2));
}

//Returns the dot product of v1 and v2
double dotProd(Vector v1, Vector v2) {
    return v1.x*v2.x + v1.y*v2.y;
}

//Returns the angle in radians between v1 and v2
double angle(Vector v1, Vector v2) {
    return acos(dotProd(v1,v2)/(norm(v1)*norm(v2)));
}

//Returns the incident angle (in radians) between the ray, defined by p1 and p2, and the normal of the wall w
double incidenceAngle(Vector p1, Vector p2, Wall w) {
    Vector ray = newVecSub(p2,p1);
    Vector wall = {w.x2 - w.x1, w.y2 - w.y1};
    return M_PI/2 - acos(fabs(dotProd(ray,wall)/(norm(ray)*norm(wall))));
}

//Returns the incident angle (in radians) between the two walls m1 and m2
double incidenceAngle2(Wall m1, Wall m2) {
    Vector p1 = newVec(m1.x1, m1.y1);
    Vector p2 = newVec(m1.x2, m1.y2);
    return incidenceAngle(p1, p2, m2);
}

//Returns the intersection point of the line defined by the 2 points p1,p2 with the wall w
Vector intersec(Vector p1, Vector p2, Wall w) {
    Wall w2 = newWall(p1.x,p1.y,p2.x,p2.y,0,0,0);
    Vector res = {0,0};
    if(w.b * w2.b != 0 && w.a != w2.a) {
        res.x = (w.c - w2.c)/(w2.a - w.a);
        res.y = w.a * res.x + w.c;
    }
    else if(w.b == 0 && w2.b != 0) {
        res.x = -w.c;
        res.y = w2.a * res.x + w2.c;
    }
    else if(w.b != 0 && w2.b == 0) {
        res.x = -w2.c;
        res.y = w.a * res.x + w.c;
    }
    return res;
}

//Returns the intersection point of two walls
Vector intersec2(Wall m1,Wall m2){
    Vector p1 = newVec(m1.x1, m1.y1);
    Vector p2 = newVec(m1.x2, m1.y2);
    return intersec(p1,p2,m2);
}

/*
 *  Returns the reflection of a point over a line
 *  v the coordinates (x,y) of the point being reflected and w the line (wall) over which the point
 *  v is reflected
 */
Vector reflecLine(Vector v, Wall w) {
    Vector reflecPt = newVec(w.x2-w.x1,w.y2-w.y1);
    Vector temp = newVec(v.x-w.x1,v.y-w.y1);
    double var = 2*dotProd(temp,reflecPt)/dotProd(reflecPt,reflecPt);
    reflecPt.x *= var;      reflecPt.y *= var;
    reflecPt.x -= temp.x;   reflecPt.y -= temp.y;
    reflecPt.x += w.x1;     reflecPt.y += w.y1;
    return reflecPt;
}

//Returns a boolean to determine whether the wall is in the way or not
static bool isBetween(Vector v, Vector w, Vector inter){
    return inter.x >= MIN(v.x,w.x) && inter.x <= MAX(v.x,w.x) && inter.y >= MIN(v.y,w.y) && inter.y <= MAX(v.y,w.y)
        && !(inter.x == v.x && inter.y == v.y) && !(inter.x == w.x && inter.y == w.y);
}

//Returns a boolean to determine wether the intersection point is located in the square ]x1;x2[x]y1;y2[ or not
static bool isOnTheWall(Vector inter, Wall m){
    return inter.x >= m.x1 && inter.x <= m.x2 && inter.y >= MIN(m.y1,m.y2) && inter.y <= MAX(m.y1,m.y2)
        && !(inter.x == m.x1 && inter.y == m.y1) && !(inter.x == m.x2 && inter.y == m.y2);
}

//Returns a boolean to determine wether the point is located on the wall or not
static bool isACorner(Vector v, Wall m){
    return (v.x == m.x1 && v.y == m.y1) || (v.x == m.x2 && v.y == m.y2);
}

/*
 *  Returns a boolean to determine wether the wall is located between the points v and w or not
 *
 *  v = absolute position of antenna
 *  w = point of calculation
 *  m = wall considered for transmission
 */
bool wallInTheWay(Vector v, Vector w, Wall m) {
    Vector inter = intersec(v, w, m);
    //printf("inter: inter_x = %2f, inter_y = %2f\n", inter.x,inter.y);
    if(isBetween(v, w, inter)) {
        return isOnTheWall(inter, m);
    }
    else {
        return false;
    }
}

/*  Returns a boolean to determine wether the potentially reflecting wall is located between v and w or not
 *
 *  v = absolute position of antenna
 *  w = point of calculation
 *  m = wall considered for reflection
 */
bool wallInReflexion(Vector v, Vector w, Wall m){
    Vector ReflecPt = reflecLine(v, m);
    return wallInTheWay(ReflecPt, w, m);
}

