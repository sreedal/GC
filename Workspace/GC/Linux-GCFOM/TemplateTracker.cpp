
#include "stdafx.h"
TemplateTracker::TemplateTracker(double swr,double dt)
{
	searchWindowRatio=swr;
	templateImage=0;
	lastLocation=0;
	detectionThreshold=dt;
	originalTemplateImage=0;
}

TemplateTracker::TemplateTracker(int tw,int th,double swr,double dt)
{
	templateWidth=tw;
	templateHeight=th;
	searchWindowWidth=(int)tw*swr;
	searchWindowHeight=(int)th*swr;
	templateImage=cvCreateImage(cvSize(tw,th),8, 1);
	originalTemplateImage=cvCreateImage(cvSize(tw,th),8, 1);
	lastLocation=0;
	searchWindowRatio=swr;
	detectionThreshold=dt;
}

TemplateTracker::~TemplateTracker()
{
	if(templateImage) cvReleaseImage(&templateImage);
	if(originalTemplateImage) cvReleaseImage(&originalTemplateImage);
}

void TemplateTracker::StartTracking(IplImage* frame, CvRect* region) //Start tracking with this template for the specifiers
{
	templateWidth=region->width;
	templateHeight=region->height;
	searchWindowWidth=(int)templateWidth*searchWindowRatio;
	searchWindowHeight=(int)templateHeight*searchWindowRatio;
	templateImage=cvCreateImage(cvSize(templateWidth,templateHeight),8, 1);
	originalTemplateImage=cvCreateImage(cvSize(templateWidth,templateHeight),8, 1);
	if(lastLocation){ delete lastLocation; lastLocation=0; }
	lastLocation=new CvRect(*region);
	cvSetImageROI(frame,*lastLocation);
	cvCvtColor(frame,originalTemplateImage,CV_BGR2GRAY);
	cvCopy(originalTemplateImage,templateImage,NULL);
	cvResetImageROI(frame);
}

CvRect* TemplateTracker::getNextLocation(IplImage* frame) //Tracks the setup region into the new frame
{
	double minval,maxval;
	double alpha=0.5; // Larger alpha => larger contribution from originalTemplate
	CvPoint minloc,maxloc;
	CvPoint start;
	assert(lastLocation); assert(templateImage);
	IplImage* probImage=cvCreateImage(cvSize(searchWindowWidth-templateWidth+1,searchWindowHeight-templateHeight+1),IPL_DEPTH_32F,1);
	IplImage* gray=cvCreateImage(cvGetSize(frame),8, 1);
	cvCvtColor(frame,gray,CV_BGR2GRAY);
	start=cvPoint(lastLocation->x+lastLocation->width/2-searchWindowWidth/2+1,lastLocation->y+lastLocation->height/2-searchWindowHeight/2+1);
	if(gray->width<=start.x+searchWindowWidth || gray->height<=start.y+searchWindowHeight) return NULL;
	cvSetImageROI(gray,cvRect(start.x,start.y,searchWindowWidth,searchWindowHeight));
	cvMatchTemplate(gray,templateImage,probImage,CV_TM_SQDIFF_NORMED);
	cvMinMaxLoc(probImage,&minval,&maxval,&minloc,&maxloc,0);
	cvResetImageROI(gray);
	cvReleaseImage(&probImage);
	cvReleaseImage(&gray);
	if(lastLocation){ delete lastLocation; lastLocation=0;}
	lastLocation=new CvRect(cvRect(start.x+minloc.x,start.y+minloc.y,templateWidth,templateHeight));
	if(minval>detectionThreshold){
		return NULL;
	}
	else{
		cvSetImageROI(frame,*lastLocation);			//
		cvCvtColor(frame,templateImage,CV_BGR2GRAY);//  THE REGION OF PROBLEMS
		cvAddWeighted(originalTemplateImage,alpha,templateImage,1-alpha,0,templateImage);
		cvResetImageROI(frame);						//
		return lastLocation;
	}
}

IplImage* TemplateTracker::drawLastLocation(IplImage* frame)
{
	assert(lastLocation);
	cvRectangle(frame,cvPoint(lastLocation->x,lastLocation->y),cvPoint(lastLocation->x+lastLocation->width,lastLocation->y+lastLocation->height),cvScalar(255,0,255,0),1,8,0);
	return frame;
}