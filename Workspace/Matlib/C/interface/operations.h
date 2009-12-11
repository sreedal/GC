#include <stdio.h>

void PrintMatrix(double *matrix, int dimension){
        int i,j;
        for(i=0;i<dimension;i++){
                for(j=0;j<dimension;j++) printf("%lf ",*(matrix+i*dimension+j));
                printf("\n");
        }
}

void PrintArray(double *matrix,int dimension1,int dimension2){
        int i,j;
        for(i=0;i<dimension1;i++){
                for(j=0;j<dimension2;j++) printf("%lf ",*(matrix+i*dimension2+j));
                printf("\n");
        }
}

void SwapRows(double **A,int dimension, int i,int j){
        int a;
        printf("Swapping %d and %d rows in:\n",i,j);
        PrintMatrix(*A,dimension);
        printf("\n Output:\n");
        for(a=0;a<dimension;a++) *((*A)+i*dimension+a)=(*((*A)+i*dimension+a))+(*((*A)+j*dimension+a));
        for(a=0;a<dimension;a++) *((*A)+j*dimension+a)=(*((*A)+i*dimension+a))-(*((*A)+j*dimension+a));
        for(a=0;a<dimension;a++) *((*A)+i*dimension+a)=(*((*A)+i*dimension+a))-(*((*A)+j*dimension+a));
        PrintMatrix(*A,dimension);
}

void MultMatrix(double *A, double *B, double **result, int i,int j,int k){// Result is initialised inside this function
	int a,b,c;
	*result=(double*)malloc(sizeof(double)*i*k);
	for(a=0;a<i;a++)
		for(b=0;b<k;b++){
			*((*result)+a*k+b) = 0;
			for(c=0;c<j;c++)
				*((*result)+a*k+b) += (*(A+a*j+c))*(*(B+c*k+b));
		}
}

void LUDecompose(double *A, double** L, double **U, int dimension){
	
}
