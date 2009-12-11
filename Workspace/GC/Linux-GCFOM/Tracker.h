#ifndef __TRACKER
#define __TRACKER

class Tracker{
public:
	void StartTracking(IplImage* frame, CvRect* region); //Start tracking with this template for the specifiers
	CvRect* getNextLocation(IplImage* frame); //Tracks the setup region into the new frame
	IplImage* drawLastLocation(IplImage* frame);
protected:
	CvRect* lastLocation;
	int templateWidth,templateHeight,searchWindowWidth,searchWindowHeight;
	double searchWindowRatio,detectionThreshold;
};


#endif