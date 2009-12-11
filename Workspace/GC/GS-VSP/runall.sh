gcc gstvideosplitter.c `pkg-config --cflags --libs gstreamer-0.10` -fPIC -c
gcc -shared -Wl,-soname,libgstvideosplitter.so -o libgstvideosplitter.so gstvideosplitter.o
sudo ln -s /home/alex/Workspace/Import/Testbed/VideoSplitter-2Srcs/libgstvideosplitter.so /usr/lib/gstreamer-0.10/libgstvideosplitter.so
gst-launch-0.10 v4l2src ! ffmpegcolorspace ! videosplitter name=splitter splitter.src1 ! queue ! ffmpegcolorspace ! ximagesink splitter.src2 ! queue ! ximagesink
