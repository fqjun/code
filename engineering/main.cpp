#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
const int e = 150;
const int e1 = 11;

Mat element_2 = getStructuringElement(MORPH_RECT, Size(e1,e1));
Mat element = getStructuringElement(MORPH_RECT, Size(e,e));
int main()
{
    Mat src_img = imread("/home/jun/下载/webwxgetmsgimg.jpeg",0);

    resize(src_img,src_img,Size(640,480));
    Mat bin_img ;
    threshold(src_img,bin_img ,100,255,THRESH_BINARY);
    erode(bin_img, bin_img, element_2);
    dilate(bin_img, bin_img, element);

    imshow("bin_img",bin_img);

    imshow("src",src_img);
    waitKey(0);
    return 0;
}
