#include "plot.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>



void plot(Emetteur listEmetteurs[], int numberOfEmetteurs, Wall listWalls[], int numberOfWalls, Vector listPts[],  int power[], int numberOfPoints) {
    
    FILE *data = fopen("data.txt", "w"); //ecriture de fichier
    int debit;
    for (int i = 0; i < numberOfPoints; i++) {
		
		//if (power[i] >= 54) {debit = 54;}
		//else if (power[i] < 6) {debit = 0;}
		//else {debit = power[i];}
		  
		if (power[i] >= -20) {power[i] = -20;}  
		if (power[i] < -93) {power[i] = -93;}   
		  
		fprintf(data, "%f %f %d \n",listPts[i].x, listPts[i].y , power[i]);
		
	}
	
	
	double dx;
	double dy;
	double resolutionWall = 1000;
	for (int n = 0; n < numberOfWalls; n++ ) {
		
		dx = (listWalls[n].x2 - listWalls[n].x1) / resolutionWall;
		dy = (listWalls[n].y2 - listWalls[n].y1) / resolutionWall;
		
		for (int jj = 0; jj < resolutionWall; jj++){
		fprintf(data, "%f %f %d \n",listWalls[n].x1 + jj*dx, listWalls[n].y1 + jj*dy , -10);
		}
		
	}
	

	
	
	
	
	for (int k = 0; k < numberOfEmetteurs; k++) {
		
		fprintf(data, "%f %f %d \n",listEmetteurs[k].position.x, listEmetteurs[k].position.y , -10);
		
	}
	

	fclose(data);
	




FILE *cmd = fopen("cmd.txt", "w");
	fprintf(cmd,
		"set view map\n"
		"set size ratio .9\n"
		"set object 1 rect from graph 0, graph 0 to graph 1, graph 1 back \n "
		"set object 1 rect fc rgb 'black' fillstyle solid 1.0 \n "
		
		//"set xrange [0:40] \n"
		//"set yrange [0:40] \n"
		
		//"set cbrange [0:54] \n"
		//"set zrange [0:54] \n"
		
		//"set palette defined (-94 'black' ,-93 'navy' , -83 'dark-blue', -73 'royalblue',-63 'dark-green'," 
		//"-53 'green',-43 'salmon', -33 'orange' , -20 'yellow', -19'white') \n" // , 45'dark-red',"
		//"50 'purple', 55'dark-violet', 64'white') \n"   //,200'light-magenta',220'magenta',240'dark-magenta',260'white')"
		
		//"set arrow from first 5.5, graph 0 to first 5.5, graph 1 nohead \n"
		
		"splot 'data.txt' using 1:2:3 with points pointtype 7 pointsize 1 palette linewidth 30 \n"
		
		
		
		//"set arrow from 0,0 to 1,1 nohead lc rgb \'red\'\n"
		//"set style arrow 7 nohead ls 1 \n"
		//"set arrow from 0,15 to 20,15 \n"
		//"show arrow \n"
		
		
	
		 );
		 
	 // for(int i = 0; i < numberOfWalls; i++) {
	 //	  fprintf(cmd,
		//  "set arrow from %f,%f to %f,%f \n", listWalls[i].x1,listWalls[i].y1,listWalls[i].x2,listWalls[i].y2
		 // );
		  //}
		  
		  
	fclose(cmd);

	//exécution
	system("gnuplot -persistent cmd.txt");
	
	
}

//http://gnuplot.sourceforge.net/docs_4.2/node217.html
