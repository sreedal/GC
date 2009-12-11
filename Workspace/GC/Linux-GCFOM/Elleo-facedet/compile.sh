gcc gstopencv.c `pkg-config --cflags --libs gstreamer-0.10` -I/usr/lib/OpenCV/opencv.build/ -I/usr/local/include/opencv -Wall -fPIC -c -o gstopencv.o
gcc edgedetect/gstedgedetect.c `pkg-config --cflags --libs gstreamer-0.10` -I/usr/lib/OpenCV/opencv.build/ -I/usr/local/include/opencv -Wall -fPIC -c -o gstedgedetect.o
gcc faceblur/gstfaceblur.c `pkg-config --cflags --libs gstreamer-0.10` -I/usr/lib/OpenCV/opencv.build/ -I/usr/local/include/opencv -Wall -fPIC -c -o gstfaceblur.o
gcc facedetect/gstfacedetect.c `pkg-config --cflags --libs gstreamer-0.10` -I/usr/lib/OpenCV/opencv.build/ -I/usr/local/include/opencv -Wall -fPIC -c -o gstfacedetect.o
gcc pyramidsegment/gstpyramidsegment.c `pkg-config --cflags --libs gstreamer-0.10` -I/usr/lib/OpenCV/opencv.build/ -I/usr/local/include/opencv -Wall -fPIC -c -o gstpyramidsegment.o
gcc templatematch/gsttemplatematch.c `pkg-config --cflags --libs gstreamer-0.10` -I/usr/lib/OpenCV/opencv.build/ -I/usr/local/include/opencv -Wall -fPIC -c -o gsttemplatematch.o
gcc -shared -Wl,-soname,libgstopencv.so -o libgstopencv.so *.o -lcv -lm -lhighgui
ln -sf $PWD/libgstopencv.so /usr/lib/gstreamer-0.10/libgstopencv.so
