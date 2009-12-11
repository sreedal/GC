#include "stdafx.h"

SuperTracker::SuperTracker(Detector *eye, Detector *eyeRegion, Detector *nose,Detector *mouth, Detector *face)
{
	eyeDetector=eye;eyeRegionDetector=eyeRegion;noseDetector=nose;mouthDetector=mouth;faceDetector=face;
	ELL=new TemplateTracker(SWR,DT);ELR=new TemplateTracker(SWR,DT);ERL=new TemplateTracker(SWR,DT);ERR=new TemplateTracker(SWR,DT);NL=new TemplateTracker(SWR,DT);NR=new TemplateTracker(SWR,DT);ML=new TemplateTracker(SWR,DT);MR=new TemplateTracker(SWR,DT);
	frame=0;
	rELL=0;rELR=0;rERL=0;rERR=0;rNL=0;rNR=0;rML=0;rMR=0;
	Detected=false;
}

SuperTracker::~SuperTracker()
{
	if(eyeDetector) delete eyeDetector;
	if(eyeRegionDetector) delete eyeRegionDetector;
	if(noseDetector) delete noseDetector;
	if(mouthDetector) delete mouthDetector;
	if(faceDetector) delete faceDetector;
}

void SuperTracker::DetectAll()
{
	bool notYetFound=true;
	CvSeq* sequence;
	Detected=true;
	IplImage* mask,*dst=cvCloneImage(frame);
	CvRect r;
	int delta = 30;
	int angle = 0;

	//////while(notYetFound){
	//////	float m[6];
 //////       CvMat M = cvMat(2, 3, CV_32F, m);
	//////	int w = frame->width;
	//////	int h = frame->height;

	//////	m[0] = (float)(cos(-angle*2*CV_PI/180.));
	//////	m[1] = (float)(sin(-angle*2*CV_PI/180.));
	//////	m[3] = -m[1];
	//////	m[4] = m[0];
	//////	m[2] = w*0.5f;  
	//////	m[5] = h*0.5f;  

	//////	//cvGetQuadrangleSubPix( frame, dst, &M);
	//////	//////cvNamedWindow("HALO",1);
	//////	//////cvShowImage("HALO",frame);
	//////	//////cvWaitKey(0);
	//////	//////cvShowImage("HALO",dst);
	//////	//////cvWaitKey(0);
	//////	cvGetQuadrangleSubPix(frame,dst,&M);

	//////	angle =(int)(angle + delta);// % 360;
	//////	if(angle>=360) return;

	//////	sequence=faceDetector->DetectAll(dst);
	//////	notYetFound=(!(sequence&&sequence->total));
	//////}
	//////frame=dst;
	sequence=faceDetector->DetectAll(frame);
	mask=faceDetector->getLastMask(0.25,0.25);
	r=faceDetector->getLastRect(0.25,0.25);
	if(!(sequence&&sequence->total)) return;
#ifdef MASK
	sequence=noseDetector->DetectAll(frame,mask);
#else
	sequence=noseDetector->DetectAll(frame,r);
#endif
	if(!(sequence&&sequence->total)) return;
	mask=faceDetector->getLastMask(0.1,0.6,0.9,1.0);
	r=faceDetector->getLastRect(0.1,0.6,0.9,1.0);
	mouthDetector->setMinSize(noseDetector->getScale(GC_X),noseDetector->getScale(GC_Y)/3);
#ifdef MASK
	sequence=mouthDetector->DetectAll(frame,mask);
#else
	sequence=mouthDetector->DetectAll(frame,r);
#endif
	if(!(sequence&&sequence->total)) return;
	mask=faceDetector->getLastMask(0.1,0.1,0.9,0.6);
	r=faceDetector->getLastRect(0,0.1,1,0.6);
#ifdef MASK
	sequence=eyeRegionDetector->DetectAll(frame,mask);
#else
	sequence=eyeRegionDetector->DetectAll(frame,r);
#endif
	if(!(sequence&&sequence->total)) return;
	mask=faceDetector->getLastMask(0.1,0.1,0.9,0.6);
	r=faceDetector->getLastRect(0.1,0.1,0.9,0.6);
	eyeDetector->setMinSize(eyeRegionDetector->getScale(GC_X)/4,eyeRegionDetector->getScale(GC_Y)/3);
#ifdef MASK
	sequence=eyeDetector->DetectAll(frame,mask);
#else
	sequence=eyeDetector->DetectAll(frame,r);
#endif
	if(!(sequence&&sequence->total)) return;
	SetRegionsFromDetection();
}

bool SuperTracker::TrackAll()
{
	CvRect *temp;
	if(ValidateRegions()){
		delete rELL; delete rELR; delete rERL; delete rERR; delete rNL; delete rNR; delete rML; delete rMR; 
		rELL=0;rELR=0;rERL=0;rERR=0;rNL=0;rNR=0;rML=0;rMR=0;
		temp=ELL->getNextLocation(frame);
		if(temp!=NULL) rELL=new CvRect(*temp);
		temp=ELR->getNextLocation(frame);
		if(temp!=NULL) rELR=new CvRect(*temp);
		temp=ERL->getNextLocation(frame);
		if(temp!=NULL) rERL=new CvRect(*temp);
		temp=ERR->getNextLocation(frame);
		if(temp!=NULL) rERR=new CvRect(*temp);
		temp=NL->getNextLocation(frame);
		if(temp!=NULL) rNL=new CvRect(*temp);
		temp=NR->getNextLocation(frame);
		if(temp!=NULL) rNR=new CvRect(*temp);
		temp=ML->getNextLocation(frame);
		if(temp!=NULL) rML=new CvRect(*temp);
		temp=MR->getNextLocation(frame);
		if(temp!=NULL) rMR=new CvRect(*temp);
	}
	if(ValidateRegions()) return true;
	return false;
}

bool SuperTracker::StartTrack()
{
	if(ValidateRegions()){
		ELL->StartTracking(frame,rELL);
		ELR->StartTracking(frame,rELR);
		ERL->StartTracking(frame,rERL);
		ERR->StartTracking(frame,rERR);
		NL->StartTracking(frame,rNL);
		NR->StartTracking(frame,rNR);
		ML->StartTracking(frame,rML);
		MR->StartTracking(frame,rMR);
		return true;
	}
	return false;
}

bool SuperTracker::isDetected()
{
	return Detected;
}

std::vector<CvPoint*> & SuperTracker::getTrack(int What){
	switch(What){
		case GC_EYES:
			return eyes;
		case GC_NOSE:
			return nose;
		case GC_MOUTH:
			return mouth;
	}
	return eyes;
}

void SuperTracker::nextStep(IplImage* image)
{
	bool valid=false;
	frame=image;
#ifdef DETECT_ONLY
	DetectAll();
	valid=ValidateRegions();
#endif
#ifdef MAIN_DETECT
	DetectAll();
	if(!ValidateRegions()){
		Detected=false;
		TrackAll();
	}else{
		StartTrack();
	}
#endif
#ifdef MAIN_TRACKING
	if(!TrackAll()){
		DetectAll();
		valid=StartTrack();
	}
	else{
		Detected=false;
		valid=true;
	}
#endif
	if(valid){
		CvPoint e=cvPoint((rELL->x+rELR->x+rERL->x+rERR->x)/4+rELL->width/2,(rELL->y+rELR->y+rERL->y+rERR->y)/4+rELL->height/2);
		CvPoint n=cvPoint((rNL->x+rNR->x)/2+rNL->width/2,(rNL->y+rNR->y)/2+rNL->height/2);
		CvPoint m=cvPoint((rML->x+rMR->x)/2+rML->width/2,(rML->y+rMR->y)/2+rML->height/2);
		eyes.push_back(new CvPoint(e));
		nose.push_back(new CvPoint(n));
		mouth.push_back(new CvPoint(m));
	}
}

IplImage* SuperTracker::drawStep()
{
	CvScalar GC_GREEN=cvScalar(0,255,0,0);
	CvScalar GC_RED= cvScalar(0,0,255,0);
	CvScalar GC_BLUE= cvScalar(255,0,0,0);
	CvScalar GC_BLACK =cvScalar(0,0,0,0);
	CvScalar GC_WHITE =cvScalar(255,255,255,0);
	CvScalar GC_CYAN =cvScalar(255,255,0,0);
	CvScalar GC_MAGENTA= cvScalar(255,0,255,0);
	CvScalar GC_YELLOW= cvScalar(0,255,255,0);

	IplImage *temp;
	if(Detected){
		temp=faceDetector->MarkAll(GC_GREEN);
		temp=mouthDetector->MarkAll(GC_RED,temp,1);
		temp=noseDetector->MarkAll(GC_BLUE,temp,1);
		temp=eyeRegionDetector->MarkAll(GC_WHITE,temp,1);
		temp=eyeDetector->MarkAll(GC_BLACK,temp,2);
	}else{
		temp=(IplImage*)cvClone(frame);
		ELL->drawLastLocation(temp);
		ELR->drawLastLocation(temp);
		ERL->drawLastLocation(temp);
		ERR->drawLastLocation(temp);
		NL->drawLastLocation(temp);
		NR->drawLastLocation(temp);
		ML->drawLastLocation(temp);
		MR->drawLastLocation(temp);
	}
	return temp;
}

bool SuperTracker::checkBounds(CvRect *r){
	return ((r->x+r->width < frame->width) && (r->x >= 0) && (r->y >=0) && (r->y+r->height < frame->height));
}

bool SuperTracker::ValidateRegions()
{
	if(!(rELL && rELR && rERL && rERR && rNL && rNR && rML && rMR)) return false; //All regions must be available
	//Add geometric constraints here
	if(!(checkBounds(rELL) && checkBounds(rELR) && checkBounds(rERL) && checkBounds(rERR )&& checkBounds(rNL) && checkBounds(rNR) && checkBounds(rML) && checkBounds(rMR))) return false; //All regions must be available
	//Eyes->Nose == Nose->Mouth
	//if(abs((rELL->y-rNL->y)-(rNL->y-rML->y))>(rELL->height/1.0)) return false;
	//if(abs(rERL->y-rNR->y-(rNR->y-rMR->y))>rERL->height/1.0) return false;
	//Symmetry (Eyes)
	//if(abs(rELL->x-rELR->x-(rERL->x-rERR->x))>rELL->width/1.0) return false;

	//if(abs(rELL->y-rNL->y-(rNL->y-rML->y))<rELL->height/10.0) return false;
	//if(abs(rELR->y-rNR->y-(rNR->y-rMR->y))<rELR->height/10.0) return false;
	AdjustTrackers();
	return true;
}

void SuperTracker::Adjust(CvRect *rec, double hGrad)
{
	if(rec==rELL){
			rELL->x=rELR->x-(rERR->x-rERL->x);
			rELL->y=rELR->y-(rERR->x-rERL->x)*hGrad;
	}else if(rec==rELR){
			rELR->x=rELL->x+(rERR->x-rERL->x);
			rELR->y=rELL->y+(rERR->x-rERL->x)*hGrad;
	}else if(rec==rERL){
			rERL->x=rERR->x-(rELR->x-rELL->x);
			rERL->y=rERR->y-(rELR->x-rELL->x)*hGrad;
	}else if(rec==rERR){
			rERR->x=rERL->x+(rELR->x-rELL->x);
			rERR->y=rERL->y+(rELR->x-rELL->x)*hGrad;
	}else if(rec==rNL){
//			printf("Support for NL,NR,ML,MR not implemented yey\n");
	}else if(rec==rNR){
//			printf("Support for NL,NR,ML,MR not implemented yey\n");
	}else if(rec==rML){
//			printf("Support for NL,NR,ML,MR not implemented yey\n");
	}else if(rec==rMR){
//			printf("Support for NL,NR,ML,MR not implemented yey\n");
	}
}

void SuperTracker::AdjustTrackers()
{
	double gradHoriz,gradVert,nw=3.0,mw=3.0,pvar[8],var[8];
	double temp,m,c,sx,sy,sxy,sx2;
	CvRect *tmpRect,*list[8];
	CvPoint c1,c2,c3;
	////////////Calculate average gradient for the horizontal axis
	//////////gradHoriz= //Eyes
	//////////	((rELL->y - rELR->y)/(rELL->x - rELR->x) +
	//////////	(rELL->y - rERL->y)/(rELL->x - rERL->x) +
	//////////	(rELL->y - rERR->y)/(rELL->x - rERR->x) +
	//////////	(rELR->y - rERL->y)/(rELR->x - rERL->x) +
	//////////	(rELR->y - rERR->y)/(rELR->x - rERR->x) +
	//////////	(rERL->y - rERR->y)/(rERL->x - rERR->x) +
	//////////			//Nose
	//////////	nw*(rNL->y - rNR->y)/(rNL->x - rNR->x) +
	//////////			//Mouth
	//////////	mw*(rML->y - rMR->y)/(rML->x - rMR->x))/(6.0+nw+mw);
	//Calculate central line
	c1.x=(rELL->x+rELR->x+rERL->x+rERR->x)/4.0 +((rELL->width+rELR->width+rERL->width+rERR->width)/4)/2;
	c1.y=(rELL->y+rELR->y+rERL->y+rERR->y)/4.0;//+((rELL->height+rELR->height+rERL->height+rERR->height)/4)/2;
	c2.x=(rNL->x+rNR->x)/2.0+((rNL->width+rNR->width)/2)/2;
	c2.y=(rNL->y+rNR->y)/2.0;//+((rNL->height+rNR->height)/2)/2;
	c3.x=(rML->x+rMR->x)/2.0+((rML->width+rMR->width)/2)/2;
	c3.y=(rML->y+rMR->y)/2.0;//+((rML->height+rMR->height)/2)/2;

	cvRectangle(frame,cvPoint(c1.x-5,c1.y-5),cvPoint(c1.x+5,c1.y+5),cvScalar(255,255,255,0),1,8,0);
	cvRectangle(frame,cvPoint(c2.x-5,c2.y-5),cvPoint(c2.x+5,c2.y+5),cvScalar(0,0,255,0),1,8,0);
	cvRectangle(frame,cvPoint(c3.x-5,c3.y-5),cvPoint(c3.x+5,c3.y+5),cvScalar(0,255,0,0),1,8,0);

	sx=c1.x+c2.x+c3.x;
	sy=c1.y+c2.y+c3.y;
	sxy=c1.x*c1.y+c2.x*c2.y+c3.x*c3.y;
	sx2=c1.x*c1.x+c2.x*c2.x+c3.x*c3.x;

	m=((3.0*sxy-sx*sy)/(3.0*sx2-sx*sx));
	c=((sy*sx2-sx*sxy)/(3.0*sx2-sx*sx));

	//Project c1,c2,c3
	c1.y=c1.x*m+c;
	c2.y=c2.x*m+c;
	c3.y=c3.x*m+c;

	//Calculate l2 = line through eyes
	sx=c1.x+rELL->x+rELR->x+rERL->x+rERR->x;
	sy=c1.y+rELL->y+rELR->y+rERL->y+rERR->y;
	sxy=c1.y*c1.x+rELL->y*rELL->x+rELR->y*rELR->x+rERL->y*rERL->x+rERR->y*rERR->x;
	sx2=c1.x*c1.x+rELL->x*rELL->x+rELR->x*rELR->x+rERL->x*rERL->x+rERR->x*rERR->x;

	m=((5.0*sxy-sx*sy)/(5.0*sx2-sx*sx));
	//m=0.0-1/m;
	c=((sy*sx2-sx*sxy)/(5.0*sx2-sx*sx));
	//c=((c1.y-m*c1.x)+(rELL->y-m*rELL->x)+(rELR->y-m*rELR->x)+(rERL->y-m*rERL->x)+(rERR->y-m*rERR->x))/5.0;

	rELL->y=rELL->x*m+c;
	rELR->y=rELR->x*m+c;
	rERL->y=rERL->x*m+c;
	rERR->y=rERR->x*m+c;

	//////////////////////////////Calculate l3 = line through nose
	////////////////////////////sx=rNL->x+rNR->x+c2.x;
	////////////////////////////sy=rNL->y+rNR->y+c2.y;
	////////////////////////////sxy=rNL->x*rNL->y+rNR->x*rNR->y+c2.x*c2.y;
	////////////////////////////sx2=rNL->x*rNL->x+rNR->x*rNR->x+c2.x*c2.x;

	////////////////////////////m=((3.0*sxy-sx*sy)/(3.0*sx2-sx*sx));
	////////////////////////////c=((sy*sx2-sx*sxy)/(3.0*sx2-sx*sx));

	//////////////////////////////rNL->y=rNL->x*m+c;
	//////////////////////////////rNR->y=rNR->x*m+c;
	////////////////////////////rNL->x=(rNL->y-c)/m;
	////////////////////////////rNR->x=(rNR->y-c)/m+rNR->width/2.0;

	//////////////////////////////Calculate l4 = line through mouth
	////////////////////////////sx=rML->x+rMR->x+c3.x;
	////////////////////////////sy=rML->y+rMR->y+c3.y;
	////////////////////////////sxy=rML->x*rML->y+rMR->x*rMR->y+c3.x*c3.y;
	////////////////////////////sx2=rML->x*rML->x+rMR->x*rMR->x+c3.x*c3.x;

	////////////////////////////m=((3.0*sxy-sx*sy)/(3.0*sx2-sx*sx));
	////////////////////////////c=((sy*sx2-sx*sxy)/(3.0*sx2-sx*sx));

	//////////////////////////////rML->y=rML->x*m+c;
	//////////////////////////////rMR->y=rMR->x*m+c;
	////////////////////////////rML->x=(rML->y-c)/m;
	////////////////////////////rMR->x=(rMR->y-c)/m+rMR->width/2.0;
	//////////////nx=1.0;
	//////////////ny=gradHoriz;
	//////////////temp=pow(nx*nx+ny*ny,0.5);
	//////////////nx=nx/temp;
	//////////////ny=ny/temp;
	//////////////b=(nx*(c1.x+c2.x+c3.x)+ny*(c1.y+c2.y+c3.y))/3.0;
	////////////////Calculate contribution of each point to the variation from mean
	//////////////pvar[0]=pow((rELL->y - rELR->y)/(rELL->x - rELR->x) - gradHoriz,2.0);
	//////////////pvar[1]=pow((rELL->y - rERL->y)/(rELL->x - rERL->x) - gradHoriz,2.0);
	//////////////pvar[2]=pow((rELL->y - rERR->y)/(rELL->x - rERR->x) - gradHoriz,2.0);
	//////////////pvar[3]=pow((rELR->y - rERL->y)/(rELR->x - rERL->x) - gradHoriz,2.0);
	//////////////pvar[4]=pow((rELR->y - rERR->y)/(rELR->x - rERR->x) - gradHoriz,2.0);
	//////////////pvar[5]=pow((rERL->y - rERR->y)/(rERL->x - rERR->x) - gradHoriz,2.0);
	//////////////pvar[6]=pow((rNL->y - rNR->y)/(rNL->x - rNR->x) - gradHoriz,2.0);
	//////////////pvar[7]=pow((rML->y - rMR->y)/(rML->x - rMR->x) - gradHoriz,2.0);
	//////////////var[0]=pvar[0]+pvar[1]+pvar[2]; list[0]=rELL;
	//////////////var[1]=pvar[0]+pvar[3]+pvar[4]; list[1]=rELR;
	//////////////var[2]=pvar[1]+pvar[3]+pvar[5]; list[2]=rERL;
	//////////////var[3]=pvar[2]+pvar[4]+pvar[5]; list[3]=rERR;
	//////////////var[4]=pvar[6]*nw; list[4]=rNL;//NL
	//////////////var[5]=pvar[6]*nw; list[5]=rNR;//NR
	//////////////var[6]=pvar[7]*mw; list[6]=rML;//ML
	//////////////var[7]=pvar[7]*mw; list[7]=rMR;//MR
	////////////////Sort
	//////////////for(int i=0;i<8;i++) for(int j=0;j<i;j++) if(var[i]<var[j]) {temp=var[i]; var[i]=var[j]; var[j]=temp; tmpRect=list[i]; list[i]=list[j]; list[j]=tmpRect;}
	//////////////printf("%lf %lf %lf %lf %lf %lf %lf %lf\n",var[0],var[1],var[2],var[3],var[4],var[5],var[6],var[7]);
	////////////////Adjust last 3 points (based on variation from average - max)
	//////////////Adjust(list[5],gradHoriz);
	//////////////Adjust(list[6],gradHoriz);
	//////////////Adjust(list[7],gradHoriz);
}

void SuperTracker::SetRegionsFromDetection()
{
	double aspect=0.95;
	CvRect* temp=eyeDetector->getIthDetection(1);
	if(temp==NULL) return;
	if(rELL) {delete rELL; rELL=0;}
	if(rELR) {delete rELR; rELR=0;}
	rELL=new CvRect(cvRect(temp->x,temp->y,temp->width/(2*aspect),temp->height*aspect));
	rELR=new CvRect(cvRect(temp->x+temp->width/2,temp->y,temp->width/(2*aspect),temp->height*aspect));
	delete temp;
	temp=eyeDetector->getIthDetection(2);
	if(temp==NULL) return;
	if(rERL){ delete rERL; rERL=0;}
	if(rERR){ delete rERR; rERR=0;}
	rERL=new CvRect(cvRect(temp->x,temp->y,temp->width/(2*aspect),temp->height*aspect));
	rERR=new CvRect(cvRect(temp->x+temp->width/2,temp->y,temp->width/(2*aspect),temp->height*aspect));
	delete temp;
	temp=noseDetector->getIthDetection(1);
	if(temp==NULL) return;
	if(rNL) {delete rNL; rNL=0;}
	if(rNR) {delete rNR;rNR=0;}
	rNL=new CvRect(cvRect(temp->x,temp->y,temp->width/(2*aspect),temp->height*aspect));
	rNR=new CvRect(cvRect(temp->x+temp->width/2,temp->y,temp->width/(2*aspect),temp->height*aspect));
	delete temp;
	temp=mouthDetector->getIthDetection(1);
	if(temp==NULL) return;
	if(rML) {delete rML;rML=0;}
	if(rMR) {delete rMR;rMR=0;}
	rML=new CvRect(cvRect(temp->x,temp->y,temp->width/(2*aspect),temp->height*aspect));
	rMR=new CvRect(cvRect(temp->x+temp->width/2,temp->y,temp->width/(2*aspect),temp->height*aspect));
	delete temp;
}