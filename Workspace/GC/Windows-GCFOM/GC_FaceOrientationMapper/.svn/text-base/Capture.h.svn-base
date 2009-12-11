#ifndef __CAPTURE
#define __CAPTURE

#define GC_BGR 0
#define GC_GRAY 1
#define GC_HSV 2
#define GC_HUE 3


class Capture{

public:
	Capture();		//Initialize capture interface
	Capture(char* fileName);	//Initialize Capture from avi file
	~Capture();		//Release the variables associated with the interface
	double getFPS();//Returns the FPS of the capture source
	IplImage* getNextFrame(int Type);	//Returns the next frame from the capture device specified by type(GC_BGR/GC_GRAY/GC_HSV)
	IplImage* getCurrFrame(int Type);	//Returns the current frame from the capture device in the specified format
	IplImage* getLastFrame(int Type);	//Returns the previous frame specified by type(GC_BGR/GC_GRAY/GC_HSV)

private:
	CvCapture* captureDevice;
	bool on;
	IplImage* lastFrame;
	IplImage* currFrame;
};
#endif