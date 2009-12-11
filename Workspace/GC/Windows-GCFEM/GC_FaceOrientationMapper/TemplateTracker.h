#ifndef __TEMPLATETRACKER
#define __TEMPLATETRACKER

class TemplateTracker: public Tracker{
public:
	TemplateTracker(double swr,double dt);
	TemplateTracker(int tw,int th,double swr,double dt);
	~TemplateTracker();
	void StartTracking(IplImage* frame, CvRect* region); //Start tracking with this template for the specifiers
	CvRect* getNextLocation(IplImage* frame); //Tracks the setup region into the new frame
	IplImage* drawLastLocation(IplImage* frame);

private:
	IplImage* templateImage;
	IplImage* originalTemplateImage;
};

#endif