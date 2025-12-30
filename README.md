# MJPEGWriter OPENCV4  (fork of JPery/MJPEGWriter)
OpenCV Video HTTP Streaming via MJPEG.
Based on the code found in 
[StackExchange -  CodeReview](http://codereview.stackexchange.com/questions/124321/multithreaded-mjpg-network-stream-server/156915#156915) and [Answers - OpenCV](http://answers.opencv.org/question/6976/display-iplimage-in-webbrowsers/)

Note: you have to write an image to the MJPEGWriter class before the start of the server.

## Example main with a fix image 
```C++
#include "MJPEGWriter.h"
#include "opencv2/video.hpp"
#include "opencv2/videoio/videoio.hpp"
#include<unistd.h>

int main()
{
    MJPEGWriter test(7777);

    Mat frame = cv::imread("Clown_fish_in_the_Andaman_Coral_Reef.jpg", cv::IMREAD_COLOR);
    test.start();
    test.write(frame);

    sleep(60);
    test.stop();
    
    exit(0);

}
```

## Example main with a camera

```C++
#include "MJPEGWriter.h"
#include "opencv2/video.hpp"
#include "opencv2/videoio/videoio.hpp"

int main()
{
    MJPEGWriter test(7777); //Creates the MJPEGWriter class to stream on the given port
    VideoCapture cap;
    bool ok = cap.open(0); //Opens webcam
    if (!ok)
    {
        printf("no cam found ;(.\n");
        pthread_exit(NULL);
    }
    Mat frame;
    cap >> frame;
    test.write(frame); //Writes a frame (Mat class from OpenCV) to the server
    frame.release();
    test.start(); //Starts the HTTP Server on the selected port
    while(cap.isOpened()){
        cap >> frame; 
        test.write(frame); 
        frame.release();
    }
    test.stop(); //Stops the HTTP Server
    exit(0);
}
```

## Compiling
Compile with OpenCV libraries and pthread:


```sh
g++ MJPEGWriter.cpp main.cpp -o MJPEG -I/usr/local/include/opencv4 -L/usr/local/lib -lpthread -lopencv_highgui -lopencv_core -lopencv_videoio -lopencv_imgcodecs
```
