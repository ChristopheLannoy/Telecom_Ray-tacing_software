#include "map.h"

//Precision
double resolution = 0.5;

//Size of the map
double side = 80;

//Walls
const int NumberOfWalls = 31 ;
Wall listWalls[31];

//Points
int NumberOfPoints = 25600;       // (side/resolution) * (side/resolution)
Vector listPt[25600];
double powers[25600];

//Emitters
Emetteur newEmetteur(double x, double y, double puissance){
	Emetteur emetteur;
	emetteur.position = newVec(x,y);
	emetteur.puissance = puissance;
	return emetteur;
};
const int NumberOfEmetteurs = 2;
Emetteur listEmetteurs[2];

//Creates the map
void createMap(){
	squareMap(side);
};

Wall newWall(double x1, double y1, double x2, double y2, double epsilonr, double sigma, double l) {
    Wall w;
    if (x2 >= x1) {
        w.x1 = x1;  w.y1 = y1;
        w.x2 = x2;  w.y2 = y2;
    }
    else {
        w.x1 = x2;  w.y1 = y2;
        w.x2 = x1;  w.y2 = y1;
    }
    // a.x + b.y + c = 0
    if(x2-x1 != 0) {
        w.a = (y2-y1)/(x2-x1);  w.b = -1;   w.c = y1 - w.a * x1;
    }
    else {
        w.a = 1;    w.b = 0;    w.c = -x1;
    }
    w.epsilonr = epsilonr;    w.sigma = sigma;    w.l = l;
    return w;
}

void squareMap(double mapSize) {
    //Borders of the map
    listWalls[0] = newWall(0,0,0,mapSize, 5,0.014,0.12);
    listWalls[1] = newWall(0,mapSize,mapSize,mapSize, 5,0.014,0.12);
    listWalls[2] = newWall(mapSize,mapSize,mapSize,0, 5,0.014,0.12);
    listWalls[3] = newWall(mapSize,0,0,0, 5,0.014,0.12);
    
    //Vertical walls
    listWalls[4] = newWall(8,0,8,48,4.6,0.02,0.2);
    listWalls[5] = newWall(8,60,8,80,4.6,0.02,0.2);
    listWalls[6] = newWall(20,72,20,80,4.6,0.02,0.2);
    listWalls[7] = newWall(24,0,24,32,5,0.014,0.4);          
    listWalls[8] = newWall(32,0,32,32,5,0.014,0.4);          
    listWalls[9] = newWall(32,40,32,60,4.6,0.02,0.2);
    listWalls[10] = newWall(40,60,40,80,4.6,0.02,0.2);
    listWalls[11] = newWall(48,0,48,60,5,0.014,0.4);         
    listWalls[12] = newWall(60,36,60,40,2.25,0.04,0.2);      
    listWalls[13] = newWall(72,20,72,24,2.25,0.04,0.2);      
    
    //Horizontal walls
    listWalls[14] = newWall(0,8,8,8,4.6,0.02,0.2);
    listWalls[15] = newWall(24,8,32,8,4.6,0.02,0.2);
    listWalls[16] = newWall(0,16,8,16,4.6,0.02,0.2);
    listWalls[17] = newWall(24,16,32,16,4.6,0.02,0.2);
    listWalls[18] = newWall(0,24,8,24,4.6,0.02,0.2);
    listWalls[19] = newWall(24,24,32,24,4.6,0.02,0.2);
    listWalls[20] = newWall(48,20,80,20,4.6,0.02,0.2);
    listWalls[21] = newWall(0,32,8,32,4.6,0.02,0.2);
    listWalls[22] = newWall(24,32,32,32,5,0.014,0.4);         
    listWalls[23] = newWall(16,80,64,80,4.6,0.02,0.2);
    listWalls[24] = newWall(48,40,80,40,4.6,0.02,0.2);
    listWalls[25] = newWall(0,48,32,48,4.6,0.02,0.2);
    listWalls[26] = newWall(0,60,80,60,5,0.014,0.4);          
    listWalls[27] = newWall(12,72,20,72,4.6,0.02,0.2);
    listWalls[29] = newWall(8,32,48,32,5,0.014,0.4);          
    
    //Diagonal walls
    listWalls[28] = newWall(60,36,72,24,2.25,0.04,0.2); 
    
    //add-ons
    listWalls[30] = newWall(4,16,24,16,5,0.014,0.4);        
    
    int nbStep = ceil(mapSize/resolution);
    resolution = mapSize/nbStep;
    for(int i = 0; i < nbStep; i++) {
        for(int j = 0; j < nbStep; j++) {
            listPt[nbStep*i+j] = newVec(resolution*(0.5+j),resolution*(0.5+i));
        }
    }
    
    //Emitters
    listEmetteurs[0] = newEmetteur(20,20,P_TX);
    //listEmetteurs[1] = newEmetteur(50,50,P_TX);
    
};
