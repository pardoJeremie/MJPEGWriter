# MJPEGWriter OPENCV4 (fork of JPery/MJPEGWriter)
OpenCV Video HTTP Streaming via MJPEG.
Based on the code found in 
[StackExchange -  CodeReview](http://codereview.stackexchange.com/questions/124321/multithreaded-mjpg-network-stream-server/156915#156915) and [Answers - OpenCV](http://answers.opencv.org/question/6976/display-iplimage-in-webbrowsers/)

This fork add:
* And update to the code to work with opencv 4;
* New examples that do not need a connected camera;
* Only update web client page when the frame is updated.

Note: you have to write an image to the MJPEGWriter class before the start of the server.

## Example main.cpp 
Simple example looping on a small mp4 file (converted from this [gif file](https://tenor.com/fr/view/clown-fish-gif-11595728)).

Note: opencv must be compiled with gstreamer (replace the cmake configuration with the following commande in the [linux installation guide](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html))
```sh
cmake -DWITH_GSTREAMER=ON ../opencv-4.x
```

## Main code example with a fix image 
```C++
#include "MJPEGWriter.h"
#include "opencv2/video.hpp"
#include "opencv2/videoio/videoio.hpp"
#include<unistd.h>

int main()
{
    MJPEGWriter test(8080);

    Mat frame = cv::imread("Clown_fish_in_the_Andaman_Coral_Reef.jpg", cv::IMREAD_COLOR);
    test.start();
    test.write(frame);

    sleep(60);
    test.stop();
    
    exit(0);

}
```
Note: [image source](https://commons.wikimedia.org/wiki/File:Clown_fish_in_the_Andaman_Coral_Reef.jpg).

## Main code example with a camera _(compiled but not tested, example from the forked repos)_

```C++
#include "MJPEGWriter.h"
#include "opencv2/video.hpp"
#include "opencv2/videoio/videoio.hpp"

int main()
{
    MJPEGWriter test(8080); //Creates the MJPEGWriter class to stream on the given port
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
