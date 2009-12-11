#include "lpsolver.h"

double A[]={1,1,1,2,2,5,4,4,8};
double b[]={5,-2,9};

int main(){
	int i;
	double *solution;
	if(lpsolve(A,b,3,&solution)==1)
		for(i=0;i<3;i++) 
			printf("%lf ",*(solution+i));
	else
		printf("Singular Matrix - No Solution\n");
	return 0;
}
