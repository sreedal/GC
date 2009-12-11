#include "stdafx.h"

Detector::Detector(const char* haarCascadeModelFile) // Build a detector for the specified HaarCascadeModel
{
	storageMemory=cvCreateMemStorage(0); assert(storageMemory);
	haarCascade=(CvHaarClassifierCascade *)cvLoad( haarCascadeModelFile, 0, 0, 0 ); assert(haarCascade);
	lastMask=0;
	objectSequence=0;
	lastFrame=0;
	minSizex=1;
	minSizey=1;
}

Detector::~Detector()
{
	cvReleaseHaarClassifierCascade(&haarCascade);
	cvReleaseMemStorage(&storageMemory);
}
void Detector::setMinSize(int sizex,int sizey){
	minSizex=sizex;
	minSizey=sizey;
}

void Detector::setMinSize(int size){
	minSizex=size;
	minSizey=size;
}

CvSeq* Detector::DetectAll(IplImage* frame)	//Detect all occurences of the object in the frame
{
	cvClearMemStorage(storageMemory);		
	if(!lastFrame) lastFrame=(IplImage*)cvClone(frame);
	else cvCopy(frame,lastFrame);
	if(!lastMask) lastMask=(IplImage*)cvClone(frame);//cvCreateImage(cvGetSize(frame),8,1);
	objectSequence = cvHaarDetectObjects(frame, haarCascade, storageMemory,1.1,6,CV_HAAR_DO_CANNY_PRUNING,cvSize(minSizex, minSizey));
	//printf("Num of regions detected:%d\n",objectSequence->total);
	checkContinuity();
	return objectSequence;
}

CvSeq* Detector::DetectAll(IplImage* frame, IplImage* mask) //Detect occurences in the masked image
{
	cvClearMemStorage(storageMemory);
	if(!lastFrame) lastFrame=(IplImage*)cvClone(frame);
	else cvCopy(frame,lastFrame);
	IplImage* temp=(IplImage*)cvClone(frame);
	cvAnd(frame,mask,temp);//cvCopy(frame,temp,mask);
	if(!lastMask) lastMask=(IplImage*)cvClone(frame);//cvCreateImage(cvGetSize(frame),8,1);

	objectSequence = cvHaarDetectObjects(temp, haarCascade, storageMemory,1.1,6,CV_HAAR_DO_CANNY_PRUNING,cvSize(minSizex, minSizey));
	checkContinuity();
	return objectSequence;
}

CvSeq* Detector::DetectAll(IplImage* frame, CvRect r) //Detect occurences in the masked image
{
	CvRect* region;
	cvClearMemStorage(storageMemory);
	if(!lastFrame) lastFrame=(IplImage*)cvClone(frame);
	else cvCopy(frame,lastFrame);
	if(r.width && r.height) cvSetImageROI(frame,r);
	objectSequence = cvHaarDetectObjects(frame, haarCascade, storageMemory,1.1,6,CV_HAAR_DO_CANNY_PRUNING,cvSize(minSizex, minSizey));
	if(r.width && r.height){
		cvResetImageROI(frame);
		for(int i=0;i<objectSequence->total;i++){
				region=(CvRect*)cvGetSeqElem(objectSequence, i);
				region->x=region->x+r.x;
				region->y=region->y+r.y;
		}
	}
	checkContinuity();
	return objectSequence;
}

void Detector::checkContinuity(){
	MyRect mr;
	int j=0;
	for(int k=0;k<20;k++) indexVec[k]=0;
	h.UpdateTime();
	for( int i=0;i<objectSequence->total;i++){
		CvRect* r = (CvRect*)cvGetSeqElem( objectSequence, i );
		UInt faceX = cvRound(r->x);
		UInt faceY = cvRound(r->y);
		UInt faceW = cvRound(r->width);
		UInt faceH = cvRound(r->height);
		mr.x = faceX; mr.y = faceY; mr.w = faceW; mr.h = faceH;
		if(!h.CheckContinuity(mr)) //Caution: mr might be overwritten by design.*/ 
		{	
			indexVec[j]=i;
			j++;
		}
	}

}

CvRect Detector::getLastRect()
{
	if(objectSequence && objectSequence->total){
		CvRect* r=(CvRect*)cvGetSeqElem(objectSequence,indexVec[0]);
		return cvRect(r->x,r->y,r->width,r->height);
	}
	else return cvRect(0,0,0,0);
}

CvRect Detector::getLastRect(double a,double b)
{
	if(objectSequence && objectSequence->total) {
		CvRect *r=(CvRect*)cvGetSeqElem(objectSequence,indexVec[0]);
		return cvRect(r->x+r->width*a,r->y+r->height*b,r->width*(1-2*a),r->height*(1-2*b));
	}
	else return cvRect(0,0,0,0);
}

CvRect Detector::getLastRect(double a,double b,double c,double d)
{
	if(objectSequence && objectSequence->total) {
		CvRect *r=(CvRect*)cvGetSeqElem(objectSequence,indexVec[0]);
		return cvRect(r->x+r->width*a,r->y+r->height*b,r->width*(1-(a+1-c)),r->height*(1-(b+1-d)));
	}
	else return cvRect(0,0,0,0);
}

IplImage* Detector::getLastMask()			//Returns the last region of detetion
{
	CvRect* region;
	if(lastMask){
		cvZero(lastMask);
		for(int i=0;i<objectSequence->total;i++){
			region=(CvRect*)cvGetSeqElem(objectSequence, indexVec[i]);
			cvRectangle(lastMask,cvPoint(region->x,region->y),cvPoint(region->x+region->width,region->y+region->height),cvScalar(255,255,255,0),CV_FILLED,8,0);
		}
	}
	return lastMask;
}

IplImage* Detector::getLastMask(double a,double b)			//Returns the last region of detetion a and b should be less than 0.5
{
	CvRect* region;
	if(lastMask){
		cvZero(lastMask);
		for(int i=0;i<objectSequence->total;i++){
			region=(CvRect*)cvGetSeqElem(objectSequence, indexVec[i]);
			cvRectangle(lastMask,cvPoint(region->x+region->width*a,region->y+region->height*b),cvPoint(region->x+region->width*(1-a),region->y+region->height*(1-b)),cvScalar(255,255,255,0),CV_FILLED,8,0);
		}
	}
	return lastMask;
}


IplImage* Detector::getLastMask(double a,double b,double c,double d)			//Returns the last region of detetion a,b,c and d should be less than 1 and c>a,d>b
{
	CvRect* region;
	if(lastMask){
		cvZero(lastMask);
		for(int i=0;i<objectSequence->total;i++){
			region=(CvRect*)cvGetSeqElem(objectSequence, indexVec[i]);
			cvRectangle(lastMask,cvPoint(region->x+region->width*a,region->y+region->height*b),cvPoint(region->x+region->width*(c),region->y+region->height*(d)),cvScalar(255,255,255,0),CV_FILLED,8,0);
		}
	}
	return lastMask;
}


IplImage* Detector::MarkAll(CvScalar color){
	CvRect* region;
	IplImage* img=(IplImage*)cvClone(lastFrame);
	for(int i=0;i<objectSequence->total;i++){
		region=(CvRect*)cvGetSeqElem(objectSequence, indexVec[i]);
		cvRectangle(img,cvPoint(region->x,region->y),cvPoint(region->x+region->width,region->y+region->height),color,1,8,0);
	}
	return img;
}

IplImage* Detector::MarkAll(CvScalar color, IplImage* img, int k){
	CvRect* region;
	if(objectSequence){
		for(int i=0;i<(k>objectSequence->total?objectSequence->total:k);i++){
			region=(CvRect*)cvGetSeqElem(objectSequence, indexVec[i]);
			cvRectangle(img,cvPoint(region->x,region->y),cvPoint(region->x+region->width,region->y+region->height),color,1,8,0);
		}
	}
	return img;
}

int Detector::getScale(int xy){
	CvRect* region=(CvRect*)cvGetSeqElem(objectSequence, indexVec[0]);
	if(region){
		if(xy=GC_X) return region->width;
		else return region->height;
	}
	else
		return 20;
}

CvRect* Detector::getIthDetection(int i){
	if(i>objectSequence->total) return NULL;
	else{
		CvRect* t=(CvRect*)cvGetSeqElem(objectSequence, indexVec[i-1]);
		return new CvRect(*t);
	}
}

//Emo Detect Perspective
IplImage* Detector::GetImage(int i){
	CvRect* region=NULL;
	if(objectSequence) region=(CvRect*)cvGetSeqElem(objectSequence, indexVec[i]);
	if(region){
		cvSetImageROI(lastFrame,*region);
		IplImage* img=(IplImage*)cvClone(lastFrame);
		cvResetImageROI(lastFrame);
		return img;
	}
	return NULL;
}