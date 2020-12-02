#ifndef plot_h
#define plot_h

#include "geometry.h"

/*
 *  Use this function to plot the points defined in listPts and the lines defined in listWalls.
 *  sizeList1 is the size of listPts and sizeList2 is the size of listWalls
 */
void plot(Emetteur listEmetteurs[], int numberOfEmetteurs, Wall listWalls[], int numberOfWalls, Vector listPts[], int power[], int numberOfPoints);

#endif
