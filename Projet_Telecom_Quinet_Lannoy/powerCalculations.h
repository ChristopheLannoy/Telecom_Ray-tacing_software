#ifndef powerCalculations_h
#define powerCalculations_h

#include <complex.h>
#include "map.h"
#include "geometry.h"

double directPathPower(Vector rx, Vector tx, double p_tx, Wall walls[], int size);
double reflecRayPower(Vector rx, Vector tx, double p_tx, Wall walls[], int size, int iteration, int j);

#endif
