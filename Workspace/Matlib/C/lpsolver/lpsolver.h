#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../interface/operations.h"

#define TRUE 1
#define FALSE 0

#define eps 0.0000001

int permute(double **A, double *b, int dimension, int pivotpos){
        int i;
        if(((*((*A)+pivotpos*dimension+pivotpos))>eps) || ((*((*A)+pivotpos*dimension+pivotpos))<(0-eps))) return 1;
        for(i=pivotpos+1;i<dimension;i++)
                if(((*((*A)+i*dimension+pivotpos))>eps) || ((*((*A)+i*dimension+pivotpos))<(0-eps))){
                        SwapRows(A,dimension,i,pivotpos);
			*(b+i)=*(b+i)+*(b+pivotpos);
			*(b+pivotpos)=*(b+i)-*(b+pivotpos);
			*(b+i)=*(b+i)-*(b+pivotpos);
                        return 1;
                }
        return 0;
}

int lpsolve(double *A, double *b, int dimension, double **solution){
        int i,j,k;
        double temp;
        double *bold,*Aold=(double*)malloc(sizeof(double)*dimension*dimension);
        bold=(double*)malloc(sizeof(double)*dimension);
        *solution=(double*)malloc(sizeof(double)*dimension);
        for(i=0;i<dimension;i++) for(j=0;j<dimension;j++) *(Aold+i*dimension+j)=*(A+i*dimension+j);
        for(i=0;i<dimension;i++) *(bold+i)=*(b+i);
        for(i=0;i<(dimension);i++){
                if(permute(&Aold,bold,dimension,i)==0) return 0;
                temp=(*(Aold+i*dimension+i));
                for(j=0;j<dimension;j++)
                        (*(Aold+i*dimension+j))=(*(Aold+i*dimension+j))/temp;
                *(bold+i)=*(bold+i)/temp;
                if(i==dimension-1) break;
                for(j=(i+1);j<dimension;j++){
                        temp=(*(Aold+(j)*dimension+i));
                        if(temp>eps || temp<(0-eps)){
                                for(k=0;k<dimension;k++)
                                        *(Aold+j*dimension+k)=(*(Aold+j*dimension+k))/temp-(*(Aold+i*dimension+k));
                                *(bold+j)=*(bold+j)/temp-*(bold+i);
                        }
                }
//              PrintMatrix(Aold,3);
//              PrintArray(bold,1,3);
        }
        for(i=dimension-1;i>=0;i--){
                *((*solution)+i)=*(bold+i);
                for(k=dimension;k>i;k--)
                        *((*solution)+i)=*((*solution)+i)-(*(Aold+i*dimension+k))*(*((*solution)+k));
        }
        free(Aold);
        free(bold);
        return 1;
}

