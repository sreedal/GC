// GC_FaceOrientationMapper.cpp : Defines the entry point for the console application.
//
#define _CRTDBG_MAP_ALLOC
#include "stdafx.h"
#include <crtdbg.h>
#define FEATURE_SIZE 20
#define CLASS_1 "outimg/S1-C1\0"
#define CLASS_2 "outimg/S1-C2\0"
#define CLASS_3 "outimg/S1-C3\0"
#define CLASS_4 "outimg/S1-C4\0"
#define CLASS_5 "outimg/S1-C5\0"

int fileCount=0,nodcount=0,shakecount=0;
CvFont font;

void PrintText(IplImage* img, char* text,CvScalar color, int l1_2){
	char* num=new char[20];
	itoa(fileCount,num,10);
//	strcat(text,num);
	if(l1_2==1){
		if(text!=NULL) cvPutText(img, text, cvPoint(10, 130), &font, color);
		else cvPutText(img, num, cvPoint(10, 130), &font, color);
	}
	else{
		if(text!=NULL) cvPutText(img, text, cvPoint(10, 230), &font, color);
		else cvPutText(img, num, cvPoint(10, 230), &font, color);
	}
	delete num;
}
void WriteVector(std::vector<CvPoint*> &vec){
	//
	std::ofstream outfile("output.track");
	for(int i=0;i<vec.size();i++){
		outfile<<vec[i]->x<<" "<<vec[i]->y<<std::endl;
	}
	outfile.close();
}

void WriteVector(std::vector<CvPoint*> &vec,int numPts,std::ofstream &outfile,int lbl){
	//
	outfile<<lbl<<" ";
	double mx=0,my=0;
	if(numPts>=vec.size()){
		for(int i=0;i<vec.size();i++){ mx+=vec[i]->x; my+=vec[i]->y; }
		mx=mx/vec.size();
		my=my/vec.size();
		for(int i=0;i<vec.size();i++){
			outfile<<2*i+1<<":"<<vec[i]->x-mx<<" "<<2*(i+1)<<":"<<vec[i]->y-my<<" ";
		}
	}else{
		for(int i=vec.size()-numPts;i<vec.size();i++){ mx+=vec[i]->x; my+=vec[i]->y; }
		mx=mx/numPts;
		my=my/numPts;
		for(int i=vec.size()-numPts;i<vec.size();i++){
			outfile<<2*(i-vec.size()+numPts)+1<<":"<<vec[i]->x-mx<<" "<<2*(i-vec.size()+numPts+1)<<":"<<vec[i]->y-my<<" ";
		}
	}
	outfile<<std::endl;
}

IplImage* DetectFacialGestures(std::vector<CvPoint*> &vec, Classifier* svm_nod, Classifier* svm_shake, Classifier* extra_class, IplImage* frame){
	std::vector<CvPoint*> Nvec;
	CvPoint *pt;
	int numPts=FEATURE_SIZE;
	double mx=0,my=0;
	for(int i=vec.size()-numPts;i<vec.size();i++){ mx+=vec[i]->x; my+=vec[i]->y; }
	//for(int i=vec.size()-1;i>=vec.size()-30;i--){
	mx=mx/numPts;
	my=my/numPts;
	for(int i=vec.size()-numPts;i<vec.size();i++){
		pt=new CvPoint(*vec[i]);
		pt->x-=mx;
		pt->y-=my;
		Nvec.push_back(pt);
	}
	if(extra_class){
		if(extra_class->Classify(Nvec,0)==1){
			printf("%d ExtraClass\n",fileCount);
			PrintText(frame,"ExtraClass\0",cvScalar(255,255,0,0),0);
		}
	}
	if(svm_nod->Classify(Nvec,0)==1){
		nodcount++;
		shakecount=0;
		if(nodcount>10){
			printf("%d:Nod\n",fileCount++);
			PrintText(frame,"NOD\0",cvScalar(255, 0, 0, 0),1);
		}else{
			printf("%d:\n",fileCount++);
			PrintText(frame,NULL,cvScalar(0, 0, 255, 0),1);
		}
	}
	else if(svm_shake->Classify(Nvec,1)==1){
		shakecount++;
		nodcount=0;
		if(shakecount>10){
			printf("%d:Shake\n",fileCount++);
			PrintText(frame,"SHAKE\0",cvScalar(0, 255, 0, 0),1);
		}else{
			printf("%d:\n",fileCount++);
			PrintText(frame,NULL,cvScalar(0, 0, 255, 0),1);
		}
	}
	else
	{
		printf("%d:\n",fileCount++);
		PrintText(frame,NULL,cvScalar(0, 0, 255, 0),1);
	}
	//////std::ofstream f("tmp.out");
	//////WriteVector(Nvec,30,f,-1);
	//////WriteVector(vec,30,f,-1);
	//////f.close();
	//////printf("wer");
	//if(svm_shake->Classify(Nvec)==1) printf("%d:Shake\n",fileCount++);
	return frame;
}



int _tmain(int argc, _TCHAR* argv[])
{
	int t=2,conv,num_class_1=0,num_class_2=0,num_class_3=0,num_class_4=0,num_class_5=0;
	double c,g,a,am=0,cs,gs;
	char input[100]="", wn[100], dn[100], filebase[100]="";
	std::ifstream tmp;
	//_crtBreakAlloc = 6000;
	//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	int q='w',lbl=-1;
	IplImage *frame;
	cvNamedWindow(ONE,1);
	cvNamedWindow(TWO,1);
	std::ofstream output("output-svm-format.data.txt");
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 2.0, 2.0, 0, 2, CV_AA);
#ifdef CAMERA
	Capture* cam=new Capture();
#else
	Capture* cam=new Capture(FILENAME);
#endif

#ifdef AAM_TRACK
	AAM_Pyramid model;
	model.ReadModel("D:\\Documents and Settings\\sreedalm\\My Documents\\AAM\\AAMLib-GSOC\\example\\my.amf");
	VJfacedetect facedet;
	facedet.LoadCascade("C:/Program Files/OpenCV/data/haarcascades/haarcascade_frontalface_default.xml");//D:/Documents and Settings/sreedalm/My Documents/AAM/AAMLib-GSOC2/example/haarcascade_frontalface_alt2.xml");
	AAM_Shape Shape;
	bool started=false;
#endif
#ifndef __DEBUG__
	Detector* faceDetector=new Detector(/*"C:/Program Files/OpenCV/data/haarcascades/*/"haarcascade_frontalface_default.xml");
	faceDetector->setMinSize(cam->getNextFrame(GC_BGR)->width/5);
	Detector* noseDetector=new Detector(/*"C:/Program Files/OpenCV/data/haarcascades/*/"Nose1.xml");
	noseDetector->setMinSize(25,15);
	Detector* mouthDetector=new Detector(/*"C:/Program Files/OpenCV/data/haarcascades/*/"Mouth.xml");
	mouthDetector->setMinSize(25,15);
	Detector* eyeRegionDetector=new Detector(/*"C:/Program Files/OpenCV/data/haarcascades/*/"frontalEyes35x16.xml");
	eyeRegionDetector->setMinSize(25,15);
	Detector* eyeDetector=new Detector(/*"C:/Program Files/OpenCV/data/haarcascades/*/"ojoD.xml");
	Blink* blinkEyeDetector=new Blink();
	SuperTracker* trackAll=new SuperTracker(eyeDetector,eyeRegionDetector,noseDetector,mouthDetector,faceDetector);
	Classifier *svm_nod=new Classifier(/*"D:\\Documents and Settings\\sreedalm\\My Documents\\Organised\\Permanent\\Work\\Libraries\\ML\\Classifiers\\SVM\\libsvm-2.89\\windows\\svm-nod-detect\\*/"model.nod.061009"/*out*/,/*"D:\\Documents and Settings\\sreedalm\\My Documents\\Organised\\Permanent\\Work\\Libraries\\ML\\Classifiers\\SVM\\libsvm-2.89\\windows\\*/"nod-scale-info.txt");
	Classifier *svm_shake=new Classifier(/*"D:\\Documents and Settings\\sreedalm\\My Documents\\Organised\\Permanent\\Work\\Libraries\\ML\\Classifiers\\SVM\\libsvm-2.89\\windows\\svm-shake-detect\\*/"model.shake.061009"/*data*/,/*"D:\\Documents and Settings\\sreedalm\\My Documents\\Organised\\Permanent\\Work\\Libraries\\ML\\Classifiers\\SVM\\libsvm-2.89\\windows\\*/"shake-scale-info.txt");
	Classifier *extra_class=NULL;
#else
	Detector* faceDetector=new Detector("C:/Program Files/OpenCV/data/haarcascades/haarcascade_frontalface_default.xml");
	faceDetector->setMinSize(cam->getNextFrame(GC_BGR)->width/5);
	Detector* noseDetector=new Detector("C:/Program Files/OpenCV/data/haarcascades/Nose1.xml");
	noseDetector->setMinSize(25,15);
	Detector* mouthDetector=new Detector("C:/Program Files/OpenCV/data/haarcascades/Mouth.xml");
	mouthDetector->setMinSize(25,15);
	Detector* eyeRegionDetector=new Detector("C:/Program Files/OpenCV/data/haarcascades/frontalEyes35x16.xml");
	eyeRegionDetector->setMinSize(25,15);
	Detector* eyeDetector=new Detector("C:/Program Files/OpenCV/data/haarcascades/ojoD.xml");
	Blink* blinkEyeDetector=new Blink();
	SuperTracker* trackAll=new SuperTracker(eyeDetector,eyeRegionDetector,noseDetector,mouthDetector,faceDetector);
	Classifier *svm_nod=new Classifier("D:\\Documents and Settings\\sreedalm\\My Documents\\Organised\\Permanent\\Work\\Libraries\\ML\\Classifiers\\SVM\\libsvm-2.89\\windows\\svm-nod-detect\\model.nod.061009"/*out*/,"D:\\Documents and Settings\\sreedalm\\My Documents\\Organised\\Permanent\\Work\\Libraries\\ML\\Classifiers\\SVM\\libsvm-2.89\\windows\\svm-nod-detect\\scale-info.txt");
	Classifier *svm_shake=new Classifier("D:\\Documents and Settings\\sreedalm\\My Documents\\Organised\\Permanent\\Work\\Libraries\\ML\\Classifiers\\SVM\\libsvm-2.89\\windows\\svm-shake-detect\\model.shake.061009"/*data*/,"D:\\Documents and Settings\\sreedalm\\My Documents\\Organised\\Permanent\\Work\\Libraries\\ML\\Classifiers\\SVM\\libsvm-2.89\\windows\\svm-shake-detect\\scale-info.txt");
	Classifier *extra_class=NULL;
#endif
	for(int i=0;i<10;i++) frame=cam->getNextFrame(GC_BGR);
	while(1){
		frame=cam->getNextFrame(GC_BGR);
		if(frame==NULL) break;
		trackAll->nextStep(frame);
#ifdef AAM_TRACK
		if(trackAll->isDetected() && faceDetector->getIthDetection(1)){ started=model.InitShapeFromDetBox(Shape, facedet, frame);}
		if(started && faceDetector->getIthDetection(1)) model.Fit(frame, Shape, 25, false);
#endif
		frame=trackAll->drawStep();
		
#ifdef AAM_TRACK
		if(started && faceDetector->getIthDetection(1)) model.Draw(frame, Shape, 1);
#endif
		cvShowImage(ONE,frame);
		//q='w';
		//q=cvWaitKey(1);
		if(q==27) break;
		if(q=='n') 
			lbl=-1;//WriteVector(trackAll->getTrack(GC_NOSE),30,output,-1);
		if(q=='p') 
			lbl=1;//WriteVector(trackAll->getTrack(GC_NOSE),30,output,1);
		if(q=='c') 
			if(t==2) 
				t=0; 
			else 
				t=2;
		{
			char temp[100]="";
			if(q=='1'){
				printf("Writing images for class 1\n");
				strcpy(filebase,CLASS_1);
				itoa(num_class_1++,temp,10);
				strcat(filebase,temp);
			}
			else if(q=='2'){
				printf("Writing images for class 2\n");
				strcpy(filebase,CLASS_2);
				itoa(num_class_2++,temp,10);
				strcat(filebase,temp);
			}
			else if(q=='3'){
				printf("Writing images for class 3\n");
				strcpy(filebase,CLASS_3);
				itoa(num_class_3++,temp,10);
				strcat(filebase,temp);
			}
			else if(q=='4'){
				printf("Writing images for class 4\n");
				strcpy(filebase,CLASS_4);
				itoa(num_class_4++,temp,10);
				strcat(filebase,temp);
			}
			else {
				printf("Writing images for class 5\n");
				strcpy(filebase,CLASS_5);
				itoa(num_class_5++,temp,10);
				strcat(filebase,temp);
			}
			trackAll->writeImages(filebase);
		}
#ifdef AUTO_TRAIN
		if(q=='t')
		{
			t=0;
			output.close();
			system("svm-scale.exe -s tmp.scale output-svm-format.data.txt > scaled.input.data");
			system("grid.py -log2c -2,2,1 -log2g -2,2,1 -svmtrain svm-train.exe -gnuplot pgnuplot.exe -s 0 -t 2 scaled.input.data");
			tmp.open("scaled.input.data.out");
			while(!tmp.fail()){
				if(a>am && c!=0){
					cs=c;
					gs=g;
					am=a;
				}
				tmp>>c>>g>>a;
				printf("%lf:%lf:%lf:->:%lf:%lf:%lf\n",c,g,a,cs,gs,am);
			}
			strcpy(input,"svm-train.exe -s 0 -t 2 -c \0");
			itoa((int)cs,wn,10);
			itoa((int)((cs-(int)cs)*1000),dn,10);
			strcat(input,wn);
			strcat(input,".\0");
			strcat(input,dn);
			strcat(input," -g \0");
			itoa((int)gs,wn,10);
			itoa((int)((gs-(int)gs)*1000),dn,10);
			strcat(input,wn);
			strcat(input,".\0");
			strcat(input,dn);
			strcat(input," scaled.input.data model.extra_class\0\n");
			printf("%s\n",input);
			getchar();
			system(input);
			if(extra_class) delete extra_class; 
			extra_class=new Classifier("model.extra_class","tmp.scale");
			output.open("output-svm-format.data.txt",std::ios_base::app);
		}
#endif
		if(trackAll->getTrack(GC_NOSE).size()>FEATURE_SIZE){
#ifdef MAIN_TRACKING
			frame=DetectFacialGestures(trackAll->getTrack(GC_NOSE),svm_nod,svm_shake,extra_class,frame);
#endif
#ifdef AUTO_TRAIN
			WriteVector(trackAll->getTrack(GC_NOSE),FEATURE_SIZE,output,lbl);
#endif
		}
		cvShowImage(TWO,frame);
		q=cvWaitKey(t);
		cvReleaseImage(&frame);
	}
	//WriteVector(trackAll->getTrack(GC_NOSE));
	//_CrtDumpMemoryLeaks();
	output.close();
	if(trackAll) delete trackAll;
	if(cam) delete cam;
	if(blinkEyeDetector) delete blinkEyeDetector;
	return 0;
}

