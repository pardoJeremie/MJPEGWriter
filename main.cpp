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
