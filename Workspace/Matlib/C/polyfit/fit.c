#include <stdio.h>
#include <math.h>

#define PI 3.1416

double x[]={0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1};
double y[11],w[15];

void fit(double *x, double *y, int degrees, double **coefficients){
	//Write out the equations to solve and call the lp-solver
}

int main(){
	int i;
	for(i=0;i<11;i++){ y[i]=sin(x[i]*2*PI);  printf("%lf ",y[i]); }
	fit(x,y,5,&w);
	return 0;
}
