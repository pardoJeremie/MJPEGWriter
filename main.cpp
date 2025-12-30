#include "MJPEGWriter.h"
#include "opencv2/video.hpp"
#include "opencv2/videoio/videoio.hpp"

int main()
{
    MJPEGWriter test(7777);

    VideoCapture cap;
    bool ok = cap.open(0);
    if (!ok)
    {
        printf("no cam found ;(.\n");
        pthread_exit(NULL);
    }
    Mat frame;
    cap >> frame;
    test.write(frame);
    frame.release();
    test.start();
    while(cap.isOpened()){cap >> frame; test.write(frame); frame.release();}
    test.stop();
    exit(0);

}
