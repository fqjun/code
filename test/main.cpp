#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main ()
{
//    Mat frame;
//    VideoCapture capture (0);
//    while(1)
//    {
//        capture >> frame;

//        imshow ("new",frame);

//        char(key)=(char)waitKey(1);
//        if(key==27)
//            break;
//    }
//     a=0;

//    for( b =300;b<=360;b++){
//        a = 360%b;
//        cout<<"a="<<a<<endl;
//    }
    Mat frame;
    Mat bin_img;
    frame = imread("/home/jun/下载/webwxgetmsgimg (2).jpeg");
    cvtColor(frame,frame,COLOR_BGR2GRAY);
    threshold(frame,bin_img,100,255,CV_8UC1);
    imshow("bin_img",bin_img);
    imwrite("/home/jun/bin.jpg",bin_img);
    waitKey(0);

    return 0;
}
