

class Classifier{
public:
	Classifier(char* modelFileName,char* scaleModelFile);
	~Classifier();
	int Classify(std::vector<CvPoint*> &vec,int toDelete);
	void loadScaleModel(char* scaleModelFile);
	char* readline(FILE *input);
	double getScaledVal(int index,double value);

private:
	int max_nr_attr;
	struct svm_node *x;
	struct svm_model* model;
	double *feature_max,*feature_min;
	double lower,upper,y_lower,y_upper;
	int max_index;
	double y_max;
	double y_min;
	int max_line_len;
	char* line;	
};