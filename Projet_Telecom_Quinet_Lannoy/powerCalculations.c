#include "powerCalculations.h"
#include <math.h>

#define EPS0 1/(36*M_PI)*1e-9   // Vacuum permittivity
#define MU0 4*M_PI*1e-7         // Vacuum permeability
#define FREQ 2.45e9             // Wi-Fi frequency
#define AIR_EPS EPS0            // Air permittivity
#define AIR_SIG 0               // Air conductivity
#define BETA 2*M_PI*FREQ/(3e8)  // Propagation constant in the air

const int NumberOfReflections = 3;  // Nombre de réflexions considérées

///////////////////////////////////////
//            BASIC FORMULAS         //
///////////////////////////////////////

//Returns the complex electric field for a direct path of length d
double complex e_field_direct(double d, double p_tx) {
    double complex e = sqrt(60*G_TX*p_tx)* cexp(-I*BETA*d)/d;
    return e;
}

//Returns the complex reflection coefficient between two mediums for a given angle theta_i
static double complex reflec_coeff(double complex epsilon1, double complex epsilon2, double theta_i) {
    double complex z1 = csqrt(MU0/epsilon1);
    double complex z2 = csqrt(MU0/epsilon2);
    double theta_t = asin(sqrt(creal(epsilon1)/creal(epsilon2)) * sin(theta_i));
    double complex coeff = (z2*cos(theta_i) - z1*cos(theta_t)) / (z2*cos(theta_i) + z1*cos(theta_t));
    return coeff;
}

//Returns the complex reflection coefficient of the wall w for a given angle theta_i
double complex reflec_coeff_wall(Wall w, double theta_i) {
    double complex epsilon1 = AIR_EPS - I*AIR_SIG/(2*M_PI*FREQ);
    double complex epsilon2 = w.epsilonr*EPS0 - I*w.sigma/(2*M_PI*FREQ);
    double theta_t = asin(sqrt(creal(epsilon1)/creal(epsilon2)) * sin(theta_i));
    double complex ref_coeff = reflec_coeff(epsilon1,epsilon2,theta_i);
    double complex gamma_m = I*2*M_PI*FREQ*csqrt(MU0*epsilon2);
    double l = w.l;
    double s = l/cos(theta_t);
    
    double complex var = cexp(-2*gamma_m*s + I*BETA*2*s*sin(theta_t)*sin(theta_i));
    double complex coeff = ref_coeff + (1-cpow(ref_coeff,2)) * ref_coeff*var/(1 - cpow(ref_coeff,2)*var);
    return coeff;
}

//Returns the complex transmission coefficient of the wall w for a given angle theta_i
double complex trans_coeff_wall(Wall w, double theta_i) {
    double complex epsilon1 = AIR_EPS - I*AIR_SIG/(2*M_PI*FREQ);
    double complex epsilon2 = w.epsilonr*EPS0 - I*w.sigma/(2*M_PI*FREQ);
    double theta_t = asin(sqrt(creal(epsilon1)/creal(epsilon2)) * sin(theta_i));
    double complex ref_coeff = reflec_coeff(epsilon1,epsilon2,theta_i);
    double complex gamma_m = I*2*M_PI*FREQ*csqrt(MU0*epsilon2);
    double l = w.l;
    double s = l/cos(theta_t);
    
    double complex var = cexp(-2*gamma_m*s + I*BETA*2*s*sin(theta_t)*sin(theta_i));
    double complex coeff = (1-cpow(ref_coeff,2)) * cexp(-gamma_m*s) / (1 - cpow(ref_coeff,2)*var);
    return coeff;
}

//Returns the average power given the electric field e_field
double average_pow(double complex e_field) {
    return 1/(8*R_A)*pow(cabs(H_E*e_field),2);
}

///////////////////////////////////////
//            TRANSMISSION           //
///////////////////////////////////////

/*
 *  Returns the transmission coefficient of the ray joining the receiver rx and the transmitter tx. This
 *  coefficient is equal to 1 when it is a direct ray. It takes also into account multiple transmissions.
 *  The arguments j and k can be used to exclude 2 walls (of index j and k), j/k = -1 when not used
 */
static double complex transCoeff(Vector rx, Vector tx, Wall walls[], int size, int j, int k) {
    double complex coeff = 1;
    for(int i = 0; i < size; i++) {
        if(i != j && i != k && wallInTheWay(rx, tx, walls[i])) {
            coeff *= trans_coeff_wall(walls[i], incidenceAngle(rx, tx, walls[i]));
        }
    }
    return coeff;
}

/*
 *  Returns the average power of the direct ray joining the receiver rx and the transmitter tx
 *  with or without transmission(s)
 */
double directPathPower(Vector rx, Vector tx, double p_tx, Wall walls[], int size) {
    double complex coeff = transCoeff(rx,tx,walls,size,-1,-1);
    Vector v = {rx.x - tx.x, rx.y - tx.y};
    double complex e_direct = coeff * e_field_direct(norm(v),p_tx);
    return average_pow(e_direct);
}

///////////////////////////////////////
//            REFLECTION             //
///////////////////////////////////////

/*
 *  Returns the reflection power for a multi-reflected ray. pts[] contains the receiver point and
 *  all the (reflected) antennas: {rx,tx,tx',tx",...}. reflecWalls[] contains the index of the walls which
 *  reflect. reflections is the number of reflections incurred
 */
static double reflectionPower(double p_tx, Wall walls[], int size, Vector pts[], int reflecWalls[], int reflections) {
    double complex coeff = 1;
    Vector pt1 = pts[0];
    Vector pt2 = pts[reflections+1];
    int i = reflections+1;
    while(i > 0) {
        Wall w = walls[reflecWalls[i]];
        // If the intersection point is not on the wall -> no reflection
        if(i > 1 && !wallInTheWay(pt2, pt1, w)) {
            coeff = 0;
            break;
        }
        else if(i > 1) {
            coeff *= reflec_coeff_wall(w, incidenceAngle(pt1, pt2, w));
            Vector inter = intersec(pt1, pt2, w);
            if(i == reflections+1) {
                // Compute the transmission coefficient from the receiver to the last
                // reflection point (excluding the wall on which it is)
                coeff *= transCoeff(inter,pt1, walls, size,reflecWalls[i],-1);
            }
            else {
                // Compute the transmission coefficient from a reflection point to another
                // (excluding the 2 walls on which they are)
                coeff *= transCoeff(inter,pt1, walls, size,reflecWalls[i],reflecWalls[i+1]);
            }
            pt1 = inter;
            pt2 = pts[i-1];
        }
        else if(i == 1) { // Compute the transmission coefficient from the first reflection point to tx
            coeff *= transCoeff(pt1, pt2, walls, size,reflecWalls[2],-1);
        }
        i--;
    }
    if(creal(coeff) != 0 && cimag(coeff) != 0) {
        Vector v = {pts[0].x - pts[reflections+1].x, pts[0].y - pts[reflections+1].y};
        double complex e_field = coeff * e_field_direct(norm(v),p_tx);
        return average_pow(e_field);
    }
    else {
        return 0;
    }
    
}

// Global variables used for computing the reflection power
static Vector pts[2+3];
static int reflecWalls[2+3];

/*
 *  Recursive function which returns the reflection power for a given receiver rx, transmitter tx
 *  and number of reflections iteration. j indicates the index of the last reflected wall in the
 *  recursivity and is used to compute the reflections below the max number of reflections iteration
 */
double reflecRayPower(Vector rx, Vector tx, double p_tx, Wall walls[], int size, int iteration, int j) {
    double power = 0;
    if(iteration == NumberOfReflections) { // Save rx and tx in pts[] which contains the receiver point and
        pts[0] = rx;            // all the (reflected) antennas: {rx,tx,tx',tx",...}.
        pts[1] = tx;
    }
    for(int i = 0; i < size; i++) {
        if(i == j) { // Reflections below the max number of reflections (iteration)
            // Example : iteration = 3, this condition computes the power of the double and single
            // reflections
            power += reflectionPower(p_tx,walls, size, pts, reflecWalls, NumberOfReflections-iteration);
        }
        else if(iteration > 1) { //Recursivity
            Vector reflecPt = reflecLine(tx, walls[i]);
            pts[2+NumberOfReflections-iteration] = reflecPt;
            reflecWalls[2+NumberOfReflections-iteration] = i;
            power += reflecRayPower(rx,reflecPt,p_tx,walls,size,iteration-1,i);
        }
        else { // iteration = 1, compute the power of the potential reflection
            Vector reflecPt = reflecLine(tx, walls[i]);
            pts[2+NumberOfReflections-iteration] = reflecPt;
            reflecWalls[2+NumberOfReflections-iteration] = i;
            power += reflectionPower(p_tx,walls, size, pts, reflecWalls, NumberOfReflections);
        }
    }
    return power;
}


