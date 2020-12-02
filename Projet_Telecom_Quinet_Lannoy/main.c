#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "map.h"
#include "geometry.h"
#include "plot.h"
#include "powerCalculations.h"
#include "main.h"

//Submission of the parameters
extern int NumberOfPoints;
extern int NumberOfWalls;
extern int NumberOfEmetteurs;
extern Vector listPt[];
extern Wall listWalls[];
extern double powers[];
extern Emetteur listEmetteurs[];

double Power(Vector tx, double p_tx, Vector pt);
void powerType(int list[], double power, int i, int type);
void powerType2(int list[], double power, int i, int type);

int main(void){
	createMap();     
	int powers_plot[NumberOfPoints];
	for(int i = 0; i < NumberOfPoints; i++) {                // Computes the power for each point of interest
            for (int k = 0; k < NumberOfEmetteurs; k++) {    // Computes the power for each transmitter
                Vector tx = listEmetteurs[k].position;
                powers[i] += Power(tx,P_TX,listPt[i]);
            }
            
            powerType2(powers_plot,powers[i],i,1);           // Convert the power to a suitable form for plotting
    }                                             
	plot(listEmetteurs,NumberOfEmetteurs,listWalls,NumberOfWalls,listPt, powers_plot, NumberOfPoints);    //plots the power
}

//First conversion of the power to a plotable value
void powerType(int list[], double power, int i, int type) {
    list[i] = 2.4 *(10*log10(power/0.001)) + 229.2;
}

//Second conversion of the power to a plotable value
void powerType2(int list[], double power, int i, int type) {
    list[i] = (10*log10(power/0.001));   
}

//Calculate the power received at a specific point pt from the emitter tx
double Power(Vector tx, double p_tx, Vector pt) {
    double power = 0;
    power += directPathPower(pt,tx,p_tx,listWalls,NumberOfWalls);
    power += reflecRayPower(pt, tx, p_tx, listWalls, NumberOfWalls, 3, -1);
    return power;
}
