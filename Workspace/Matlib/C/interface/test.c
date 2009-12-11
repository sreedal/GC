#include "operations.h"
int main(){
	double A[]={1,2,3,4,5,6},B[]={1,1,1,1,2,2,2,2,3,3,3,3},*result;
	MultMatrix(A,B,&result,2,3,4);
	PrintArray(result,2,4);
	return 0;
}
