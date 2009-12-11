#ifndef __BLINK
#define __BLINK

#define BORDER 10

class Blink{
public:
	Blink();
	~Blink();
	void Detect(IplImage* newFrame);
	void TrackEyes(IplImage* newFrame,IplImage* mask);
	IplImage* MarkAll(CvScalar color);

private:
	CvRect *leftEye,*rightEye;
	IplImage* prev;
	IplImage* curr;
	IplConvKernel* kernel;
	TemplateTracker *leftEyeTracker,*rightEyeTracker;
	IplImage* oriImage;
	IplImage* tmp;
};

#endif