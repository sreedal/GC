#include "stdafx.h"

Capture::Capture()			//Initialize capture interface
{
	captureDevice = cvCaptureFromCAM( CV_CAP_ANY );
	assert(captureDevice!=NULL);
	on=false;
	lastFrame=0;
	currFrame=0;
}

Capture::~Capture()			//Release the variables associated with the interface
{
	cvReleaseCapture(&captureDevice);
}

Capture::Capture(char* fileName)
{
	captureDevice =cvCaptureFromAVI(fileName);
	assert(captureDevice!=NULL);
	on=false;
	lastFrame=0;
	currFrame=0;
}

double Capture::getFPS()	//Returns the FPS of the capture source
{
	return cvGetCaptureProperty(captureDevice,CV_CAP_PROP_FPS);
}

IplImage* Capture::getNextFrame(int Type)	//Returns the next frame from the capture device specified by type(GC_BGR/GC_GRAY/GC_HSV)
{
	IplImage *img=0,*temp=0;
	lastFrame=currFrame;
	currFrame=cvQueryFrame(captureDevice);
	//assert(currFrame);
	if(lastFrame==NULL && currFrame!=NULL){
		lastFrame=(IplImage*)cvClone(currFrame);
		cvZero(lastFrame);
	}
	if(currFrame){
		switch(Type){
			case GC_BGR:
				img=(IplImage*)cvClone(currFrame);
				break;
			case GC_GRAY:
				img=cvCreateImage(cvGetSize(currFrame),8,1);
				cvCvtColor(currFrame,img,CV_BGR2GRAY);
				break;
			case GC_HSV:
				img=cvCreateImage(cvGetSize(currFrame),8,3);
				cvCvtColor(currFrame,img,CV_BGR2HSV);
				break;
			case GC_HUE:
				temp=cvCreateImage(cvGetSize(currFrame),8,3);
				img=cvCreateImage(cvGetSize(currFrame),8,1);
				cvCvtColor(currFrame,temp,CV_BGR2HSV);
				cvSplit(temp, img, 0, 0, 0 );
				break;
		} 
	}
	else{
		return NULL;
	}
	if(img->origin==1){
		cvFlip( img, 0, 0 );
		img->origin = 0;
	}
	return img;
}

IplImage* Capture::getCurrFrame(int Type)	//Returns the current frame in the specified format
{
	IplImage *img=0,*temp=0;
	if(currFrame){
		switch(Type){
			case GC_BGR:
				img=(IplImage*)cvClone(currFrame);
				break;
			case GC_GRAY:
				img=cvCreateImage(cvGetSize(currFrame),8,1);
				cvCvtColor(currFrame,img,CV_BGR2GRAY);
				break;
			case GC_HSV:
				img=cvCreateImage(cvGetSize(currFrame),8,3);
				cvCvtColor(currFrame,img,CV_BGR2HSV);
				break;
			case GC_HUE:
				temp=cvCreateImage(cvGetSize(currFrame),8,3);
				img=cvCreateImage(cvGetSize(currFrame),8,1);
				cvCvtColor(currFrame,temp,CV_BGR2HSV);
				cvSplit(temp, img, 0, 0, 0 );
				break;
		}
	} 
	if(img->origin==1){
		cvFlip( img, 0, 0 );
		img->origin = 0;
	}
	return img;
}

IplImage* Capture::getLastFrame(int Type)	//Returns the previous frame specified by type(GC_BGR/GC_GRAY/GC_HSV)
{
	
	IplImage *img=0,*temp=0;
	if(lastFrame){
		switch(Type){
			case GC_BGR:
				img=(IplImage*)cvClone(lastFrame);
				break;
			case GC_GRAY:
				img=cvCreateImage(cvGetSize(lastFrame),8,1);
				cvCvtColor(lastFrame,img,CV_BGR2GRAY);
				break;
			case GC_HSV:
				img=cvCreateImage(cvGetSize(lastFrame),8,3);
				cvCvtColor(lastFrame,img,CV_BGR2HSV);
				break;
			case GC_HUE:
				temp=cvCreateImage(cvGetSize(lastFrame),8,3);
				img=cvCreateImage(cvGetSize(lastFrame),8,1);
				cvCvtColor(lastFrame,temp,CV_BGR2HSV);
				cvSplit(temp, img, 0, 0, 0 );
				break;
		} 
	}
	if(img->origin==1){
		cvFlip( img, 0, 0 );
		img->origin = 0;
	}
	return img;
}