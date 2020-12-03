#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int g_value = 50;


int main()
{
    VideoCapture capture (0);
    Mat src;

    Mat gray_img;

    namedWindow("bar");
    createTrackbar("value","bar",&g_value,255,nullptr);
    while(1)
    {

        capture >> src;
        imshow("11",src);


        cvtColor(src,gray_img,COLOR_BGR2GRAY);

        threshold(gray_img,gray_img,g_value,255,THRESH_BINARY);

        imshow("gray",gray_img);

        char(key)=(char)waitKey(1);
        if(key==27)
            break;
    }
    return 0;
}
