#include "MJPEGWriter.h"
#include "opencv2/video.hpp"
#include "opencv2/videoio/videoio.hpp"
#include <unistd.h>
#include <thread>
#include <atomic>

#define DEBUG // print logs if defined 
#define VIDEOFILEPATH "clown-fish.mp4" // path to the video file to loop
#define WEB_PAGE_LIFESPAN 30 // in second

std::atomic<bool> sleep_finished;

// set the while existe condition 'sleep_finished' to true after a 'sleep_time_s' second of sleep
void sleep_thread_func(uint sleep_time_s) {
    sleep_finished = false;
#ifdef DEBUG
    std::cout << "debug: sleep_thread_func will sleep for " << sleep_time_s << "s" << std::endl;
#endif
    sleep(sleep_time_s);
    sleep_finished = true;
}

// Check if the video was opened successfully
bool open_video_file(cv::VideoCapture *cap, std::string path) {
    cap->open(path);
    if (!cap->isOpened()) {
#ifdef DEBUG
        std::cout << "Error: Could not open video file." << std::endl;
#endif
        return false;
    }
#ifdef DEBUG
    std::cout << "Video file opened successfully!" << std::endl;
#endif
    return true;
}

int main()
{
    MJPEGWriter test(8080);
    // Open the video file
    cv::VideoCapture cap;
 
    // Check if the video was opened successfully
    if (!open_video_file(&cap, VIDEOFILEPATH)) {
        return -1;
    }

    // get the needed sleep time to respect the video frame rate
    uint framewait = 1000000 / cap.get(cv::CAP_PROP_FPS);
#ifdef DEBUG
    std::cout << "debug: the fps of the video is " << cap.get(cv::CAP_PROP_FPS) << "fps" << std::endl;
    std::cout << "debug: the wait between the frame will be " << framewait << "ns" << std::endl;
#endif

    // start the web server
    test.start();

    // sleep for a 'WEB_PAGE_LIFESPAN' s before existing the following loop
    std::thread sleep_thread(sleep_thread_func,WEB_PAGE_LIFESPAN);

    // Read and display each frame of the video in loop still the sleep_thread is down
    while (!sleep_finished) {
        cv::Mat frame;

        if (!cap.read(frame)) {
            // -- NORMAL WAY OF LOOPING --
            // Do not work for me, i end with the following error
            //  global cap_gstreamer.cpp:2022 setProperty OpenCV | GStreamer warning: unable to seek
            // cap.set(cv::CAP_PROP_POS_FRAMES , 0); // looping the video file

            // -- DISGUSTING WAY TO DO LOOPING --
            cap.release ();
            if (!open_video_file(&cap, VIDEOFILEPATH)) {
                return -1;
            }
            continue;
        }

        // Display the frame to the web page
        test.write(frame); 
        // sleep the determined duration of the frame  
        // Attention! the frame rate will be framewait + computational cost of the above code in the loop
        usleep(framewait); 
    }
    
    // stop the web server
    test.stop();
    cap.release ();

    return 0; // end of program
}
