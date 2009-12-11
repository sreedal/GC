#ifndef __DETECTOR
#define __DETECTOR

#define GC_X 0
#define GC_Y 1

class Detector{
public:
	Detector(const char* haarCascadeModelFile); // Build a detector for the specified HaarCascadeModel
	~Detector();
	CvSeq* DetectAll(IplImage* frame);	//Detect all occurences of the object in the frame
	CvSeq* DetectAll(IplImage* frame, IplImage* mask); //Detect occurences in the masked image
	CvSeq* DetectAll(IplImage* frame, CvRect r);
	IplImage* getLastMask();			//Returns the last region of detetion
	CvRect getLastRect();
	CvRect getLastRect(double a,double b,double c,double d);
	CvRect getLastRect(double a,double b);
	IplImage* getLastMask(double a,double b,double c,double d);//Returns the last region of detetion a,b,c and d should be less than 1
	IplImage* getLastMask(double a,double b);//Returns the last region of detetion a and b should be less than 0.5
	IplImage* MarkAll(CvScalar color); //Mark all occurences from the last detection.
	IplImage* MarkAll(CvScalar color, IplImage* img,int k);
	void setMinSize(int sizex,int sizey); //Set the minsize restrictions
	void setMinSize(int size);
	int getScale(int dimension);
	CvRect* getIthDetection(int i);

private:
	CvHaarClassifierCascade *haarCascade;
	CvMemStorage *storageMemory;
	CvSeq *objectSequence;
	IplImage* lastMask;
	IplImage* lastFrame;
	int minSizex,minSizey;
};

#endif
