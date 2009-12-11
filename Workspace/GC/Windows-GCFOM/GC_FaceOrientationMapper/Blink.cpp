#include "stdafx.h"

Blink::Blink()
{
	leftEye=NULL;
	rightEye=NULL;
	prev=NULL;
	curr=NULL;
	kernel = cvCreateStructuringElementEx(7,7,1,1,  CV_SHAPE_CROSS,NULL);  //3, 3, 1, 1, CV_SHAPE_CROSS, NULL);
	leftEyeTracker=NULL;
	rightEyeTracker=NULL;
	oriImage=NULL;
}

Blink::~Blink()
{
	if(prev) cvReleaseImage(&prev);
	if(curr) cvReleaseImage(&curr);
	if(oriImage) cvReleaseImage(&oriImage);
}

void Blink::Detect(IplImage* newFrame)
{
	CvMemStorage* storage=cvCreateMemStorage(0);
	CvSeq* contour;
	CvPoint start,end;
	if(prev==NULL && curr!=NULL){
		prev=cvCreateImage(cvSize(240,180),8,1);//cvGetSize(newFrame)
		cvCopy(curr,prev,NULL);
		cvCvtColor(newFrame,tmp,CV_BGR2GRAY);
		cvResize(tmp,curr,1);
	}
	if(curr==NULL){
		curr=cvCreateImage(cvSize(240,180),8,1);//cvGetSize(newFrame)
		tmp=cvCreateImage(cvGetSize(newFrame),8,1);
		cvCvtColor(newFrame,tmp,CV_BGR2GRAY);
		cvResize(tmp,curr,1);
		return;
	}
	if(prev && curr){
		cvCopy(curr,prev,NULL);
		cvCvtColor(newFrame,tmp,CV_BGR2GRAY);
		cvResize(tmp,curr,1);
	}
	//Find Connected Components in the difference image
	assert(curr && prev);
	IplImage* diff=cvCreateImage(cvGetSize(curr),8,1);
	cvZero(diff);
	if(leftEye && rightEye){
		start.x=leftEye->x-BORDER;
		start.y=leftEye->y-BORDER;
		end.x=rightEye->x+rightEye->width+BORDER;
		end.y=rightEye->y+rightEye->height+BORDER;
		cvSetImageROI(curr,cvRect(start.x,start.y,end.x-start.x,end.y-start.y));
		cvSetImageROI(prev,cvRect(start.x,start.y,end.x-start.x,end.y-start.y));
		cvSetImageROI(diff,cvRect(start.x,start.y,end.x-start.x,end.y-start.y));
	}
	cvSub(curr,prev,diff,NULL);
	cvThreshold(diff, diff, 5, 255, CV_THRESH_BINARY);
	cvMorphologyEx(diff, diff, NULL, kernel, CV_MOP_OPEN, 1);
	if(leftEye && rightEye){
		cvResetImageROI(curr);
		cvResetImageROI(prev);
		cvResetImageROI(diff);
	}
	cvShowImage(ONE,diff);
	cvWaitKey(1);
	int nc=cvFindContours(diff,storage,&contour,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));
	cvClearMemStorage(storage);
	cvReleaseImage(&diff);
	//Check if the components found is a eye pair
	if(nc!=2 || contour==0){
		//Detection Failed try tracking if eyepair from previous image exist
		return;
	}
	CvRect l,r;
	l=cvBoundingRect(contour,1);
	contour=contour->h_next;
	r=cvBoundingRect(contour,1);
	if(abs(l.width-r.width)>5 || abs(l.height-r.height)>5 || abs(l.y-r.y)>5 || (abs(l.x-l.y)/l.width)<2 || (abs(l.x-l.y)/l.width)>5 ){ 
		//Detection Failed
		return;
	}
	//Detected good -> setup variables for future use
	leftEye=&l;
	rightEye=&r;
	if(!leftEyeTracker) leftEyeTracker=new TemplateTracker(1.5,0.1);
	if(!rightEyeTracker) rightEyeTracker=new TemplateTracker(1.5,0.1);
	leftEyeTracker->StartTracking(newFrame,leftEye);
	rightEyeTracker->StartTracking(newFrame,rightEye);
}

void Blink::TrackEyes(IplImage* newFrame, IplImage* mask)
{
	if(!oriImage) oriImage=(IplImage*)cvClone(newFrame);
	else cvCopy(newFrame,oriImage,NULL);

	IplImage* temp=(IplImage*)cvClone(newFrame);
	if(mask!=NULL) cvAnd(newFrame,mask,temp);

	if(leftEyeTracker && rightEyeTracker){
		leftEye=leftEyeTracker->getNextLocation(temp);
		rightEye=rightEyeTracker->getNextLocation(temp);
	}
	else{
		Detect(temp);
	}
}

IplImage* Blink::MarkAll(CvScalar color)
{
	if(!oriImage) return NULL;
	IplImage* temp=(IplImage*) cvClone(oriImage);
	if(leftEye) cvRectangle(temp,cvPoint(leftEye->x,leftEye->y),cvPoint(leftEye->x+leftEye->width,leftEye->y+leftEye->height),color,1,8,0);
	if(rightEye) cvRectangle(temp,cvPoint(rightEye->x,rightEye->y),cvPoint(rightEye->x+rightEye->width,rightEye->y+rightEye->height),color,1,8,0);
	return temp;
}