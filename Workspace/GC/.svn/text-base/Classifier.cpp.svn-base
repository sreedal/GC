#include "stdafx.h"

Classifier::Classifier(char* modelFileName, char* scaleModelFile){
	model=svm_load_model(modelFileName);
	max_nr_attr=64;
	lower=-1.0;
	upper=1.0;
	y_max = -DBL_MAX;
	y_min = DBL_MAX;
	max_line_len = 1024;
	line=(char *)malloc(1024*sizeof(char));
	loadScaleModel(scaleModelFile);
}

Classifier::~Classifier(){
	svm_destroy_model(model);
	if(feature_min) free((void*)feature_min);
	if(feature_max) free((void*)feature_max);
	if(line) free((void*)line);
}

double Classifier::getScaledVal(int index,double value){
	/* skip single-valued attribute */
//	printf("%lf",value);
	if(feature_max[index] == feature_min[index]){
//		printf("%:->:%lf\n",value);
		return 0;
	}

	if(value == feature_min[index])
		value = lower;
	else if(value == feature_max[index])
		value = upper;
	else
		value = lower + (upper-lower) * 
			(value-feature_min[index])/
			(feature_max[index]-feature_min[index]);
//	printf("%:->:%lf\n",value);
	return value;
}

int Classifier::Classify(std::vector<CvPoint*> &vec,int toDelete){
	int i;
	while(max_nr_attr<(vec.size()+1)) max_nr_attr*=2;
	x=(struct svm_node *) malloc(max_nr_attr*sizeof(struct svm_node));
	for(i=0;i<(vec.size());i++){
		x[2*i].index=2*i+1;
		x[2*i+1].index=2*i+2;
		x[2*i].value=getScaledVal(2*i+1,vec[i]->x);
		x[2*i+1].value=getScaledVal(2*i+2,vec[i]->y);
		if(toDelete==1) delete vec[i];
	}
	x[2*i].index=-1;
	int lbl=svm_predict(model,x);
	//printf("%d\n",lbl);
	free((void*)x);
	return lbl;
}

int max(int a,int b){
	if(a>b) return a;
	return b;
}

void Classifier::loadScaleModel(char* scaleModelFile){
	//Max Index Computation
		int idx, c;
		FILE *fp_restore = fopen(scaleModelFile,"r");
		if(fp_restore==NULL)
		{
			fprintf(stderr,"can't open file %s\n", scaleModelFile);
			exit(1);
		}
		c = fgetc(fp_restore);
		if(c == 'y')
		{
			readline(fp_restore);
			readline(fp_restore);
			readline(fp_restore);
		}
		readline(fp_restore);
		readline(fp_restore);
		while(fscanf(fp_restore,"%d %*f %*f\n",&idx) == 1)
			max_index = max(idx,max_index);
		rewind(fp_restore);
	//Allocate Variables
		feature_max = (double *)malloc((max_index+1)* sizeof(double));
		feature_min = (double *)malloc((max_index+1)* sizeof(double));
		if(feature_max == NULL || feature_min == NULL)
		{
			fprintf(stderr,"can't allocate enough memory\n");
			exit(1);
		}
		for(int i=0;i<=max_index;i++)
		{
			feature_max[i]=-DBL_MAX;
			feature_min[i]=DBL_MAX;
		}
	//Setting Min Max Feature values
		double fmin, fmax;
		if((c = fgetc(fp_restore)) == 'y')
		{
			fscanf(fp_restore, "%lf %lf\n", &y_lower, &y_upper);
			fscanf(fp_restore, "%lf %lf\n", &y_min, &y_max);
//		y_scaling = 1;
		}
		else
			ungetc(c, fp_restore);
		if (fgetc(fp_restore) == 'x') {
			fscanf(fp_restore, "%lf %lf\n", &lower, &upper);
			while(fscanf(fp_restore,"%d %lf %lf\n",&idx,&fmin,&fmax)==3)
			{
				if(idx<=max_index)
				{
					feature_min[idx] = fmin;
					feature_max[idx] = fmax;
				}
			}
		}
		fclose(fp_restore);
}

char* Classifier::readline(FILE *input)
{
	int len;
	
	if(fgets(line,max_line_len,input) == NULL)
		return NULL;

	while(strrchr(line,'\n') == NULL)
	{
		max_line_len *= 2;
		line = (char *) realloc(line, max_line_len);
		len = (int) strlen(line);
		if(fgets(line+len,max_line_len-len,input) == NULL)
			break;
	}
	return line;
}
