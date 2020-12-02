#ifndef main_h
#define main_h

//Wall object
typedef struct Wall Wall;
struct Wall {
    double x1,y1;
    double x2,y2;
    double a;
    double b;
    double c;
    double epsilonr;
    double sigma;
    double l;
};

//Vector object
typedef struct Vector Vector;
struct Vector {
    double x;
    double y;
};

//Emitter object
typedef struct Emetteur Emetteur;
struct Emetteur {
    Vector position;
    double puissance;
};

Wall newWall(double x1, double y1, double x2, double y2, double epsilon, double sigma, double l);
void createMap();
void squareMap(double mapSize);

#endif
