#include <glylib.h>

double angle(coord_3D B,coord_3D C){
//printf("entered angle\n");

coord_3D BC;
BC=subtract_coord(B,C);

//printf("BC.i is %f\n",BC.i);
//printf("BC.j is %f\n",BC.j);
//printf("BC.k is %f\n",BC.k);

double cos;
cos = sqrt(BC.i*BC.i+BC.j*BC.j+BC.k*BC.k);

double costeta;

costeta = BC.i/cos;

//printf("costeta is %f\n",costeta);
double teta;

teta = acos(costeta);
teta = teta*(180/PI);
//printf("teta initially is %f\n",teta);
//printf("BC.i is %f and BC.j is %f\n",BC.i,BC.j);

	
if(BC.j>0){
	teta = teta;
	}
if(BC.j<0){
	teta=360-teta;
	}



//printf("teta is %f\n",teta);
teta = teta*PI/180;

//teta = acos(costeta);



return teta;

}
