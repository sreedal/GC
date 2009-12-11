// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#include <stdio.h>
#include <tchar.h>
#include <assert.h>
#include <cv.h>
#include <highgui.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <stdlib.h>

using std::vector;
#pragma once

#include "History.h"
#include "targetver.h"
#include "Capture.h"
#include "Display.h"
#include "Detector.h"
#include "Tracker.h"
#include "TemplateTracker.h"
#include "Blink.h"
#include "SuperTracker.h"
#include "Classifier.h"
#include <svm.h>


#include "AAM_IC.h"
#include "AAM_Basic.h"
#include "AAM_VJfaceDetect.h"

//#define AUTO_TRAIN
//#define MAIN_TRACKING
#define __DEBUG__
#define DETECT_ONLY
//#define MASK
#define CAMERA
#define FILENAME "D:\\Documents and Settings\\sreedalm\\My Documents\\Organised\\Permanent\\Work\\Dataset\\CV\\FacialGestures\\big-test-video.avi" //test-video.avi"   //shake1.avi" //nod1.avi" //sourjya.avi" //sreedal.avi"
//#define AAM_TRACK


// TODO: reference additional headers your program requires here

#ifndef	COMMONDEF_H
#define COMMONDEF_H

#include <stdlib.h>
#include <math.h>

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include < string >
#include < sstream >
#include < iostream >

using std::cout;
using std::endl;
using std::string;
using std::ostringstream;

// type defs
typedef void			Void;
typedef	unsigned long	ULong;
typedef	long			Long;
typedef	signed int		Int;
typedef	unsigned int	UInt;
typedef	signed short	SInt;
typedef	unsigned short	USInt;
typedef	unsigned char	UChar;
typedef	signed char		Char;
typedef	float			Float;
typedef	double			Double;
typedef bool			Bool;

#ifndef PI
#define PI					3.14159265358979323846264
#endif	//PI

#ifndef EPSILON
#define EPSILON				0.0000000000000001
#endif	//EPSILON

#ifndef MAXSTRINGLENGTH
#define MAXSTRINGLENGTH		256
#endif	//MAXSTRINGLENGTH

#ifndef FIRST_IMAGE
#define FIRST_IMAGE			1
#endif //FIRST_IMAGE

#ifndef SECOND_IMAGE
#define SECOND_IMAGE		2
#endif //SECOND_IMAGE

#ifndef TRUE
#define TRUE				1
#endif //TRUE

#ifndef FALSE
#define FALSE				0
#endif //FALSE

/*****************************************************************************
 * Function Name	: ToString(template class)
 * Description		: This function converts input into strings
 * Parameters		: T &arg - input data to be converted into a string
 * Called functions	: None				  
 * Global Data		: None
 * Return Value		: string
 * Exceptions		: None
 *****************************************************************************/
template < class T >
string ToString(const T &arg)
{
	ostringstream out;
	out << arg;
	return(out.str());
}

#endif	//COMMONDEF_H