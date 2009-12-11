#ifndef __SUPERTRACKER
#define __SUPERTRACKER


#define SWR 2
#define DT 0.05

#define GC_NOSE	0
#define GC_EYES	1
#define GC_MOUTH	2

class SuperTracker{
public:
	SuperTracker(Detector *eyeDetector, Detector *eyeRegionDetector, Detector *noseDetector,Detector *mouthDetector, Detector *faceDetector);
	~SuperTracker();
	void DetectAll();
	bool TrackAll();
	bool StartTrack();
	void nextStep(IplImage* frame);
	IplImage* drawStep();
	bool ValidateRegions();
	void AdjustTrackers();
	void SetRegionsFromDetection();
	void Adjust(CvRect*, double );
	bool checkBounds(CvRect *r);
	bool isDetected();
	std::vector<CvPoint*> & getTrack(int What);
	void writeImages(char* filename);
private:
	TemplateTracker *ELL,*ELR,*ERL,*ERR,*NL,*NR,*ML,*MR; 
	IplImage* frame;
	CvRect *rELL,*rELR,*rERL,*rERR,*rNL,*rNR,*rML,*rMR;
	Detector *eyeDetector, *eyeRegionDetector, *noseDetector, *mouthDetector, *faceDetector;
	bool Detected;
	std::vector<CvPoint*> eyes,nose,mouth;
};

#endif