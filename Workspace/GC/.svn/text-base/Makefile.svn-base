CXX = g++
CXXFLAGS = -I/usr/lib/OpenCV/OpenCV-2.0.0/include/opencv/ -I/usr/include/libsvm-2.0/libsvm/
OBJFLAGS = -lm -lcv -lhighgui -lsvm
SRC = Blink.cpp Capture.cpp  Classifier.cpp  Detector.cpp  GC_FaceOrientationMapper.cpp  SuperTracker.cpp  TemplateTracker.cpp
OBJ = $(addsuffix .o, $(basename $(SRC)))

output: $(OBJ)
	$(CXX) $(OBJ) -o output $(OBJFLAGS)

$(OBJ): $(SRC) stdafx.h
	$(CXX) $(CXXFLAGS) -c $(addsuffix .cpp, $(basename $@)) -o $@

clean:
	rm -rf *.o
