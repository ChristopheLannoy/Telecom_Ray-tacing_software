#ifndef geometry_h
#define geometry_h

#include "main.h"
#include "map.h"
#include <stdbool.h>

#define M_PI 3.1415



Vector newVec(double x, double y);
void vecSub(Vector *v1, Vector v2);
double norm(Vector v);
double angle(Vector v1, Vector v2);

double incidenceAngle(Vector p1, Vector p2, Wall w);
Vector intersec(Vector p1, Vector p2, Wall w);
Vector reflecLine(Vector v, Wall w);
bool wallInTheWay(Vector v, Vector w, Wall m);
int diffractionPcts(Vector diffrac[], Wall diffracWalls[], Wall listWalls[], int nbWalls);
Vector diffracAngle(Vector diffracPos, Wall w, Vector rx, Vector tx);

#endif


