#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

using namespace std;
using namespace cv;


Mat g_bin_img;
Mat g_gray_img;

int g_max=255;
int g_thresh=75;
int g_type=1;

void on_callback(int,void *);

int main()
{
    Mat src_img;
    Mat copy_img;

    Mat roi_img;
    Mat roi_img1;

    src_img=imread("/home/jun/workplace/任务三/Cross.jpg");
    copy_img=src_img.clone();
    Rect roi = Rect(210,47,270,310);
    src_img(roi).copyTo(roi_img);
    roi_img1=roi_img.clone();


    cvtColor(roi_img1,g_gray_img,COLOR_RGB2GRAY);





    namedWindow("滑动条");
    createTrackbar("阈值:","滑动条",&g_thresh,g_max,on_callback);
    createTrackbar("类型:","滑动条",&g_type,4,on_callback);
    on_callback(0,0);




    vector<Vec2f>lines;
    HoughLines(g_bin_img,lines,1,CV_PI/180,200,0,0);

    cout<<lines.size()<<endl;

    for(size_t i=0;i<lines.size();i++)
    {
        float rho = lines[i][0],theta = lines[i][1];
        Point pt1,pt2;
        double a = cos(theta),b= sin(theta);
        double x0 = a*rho,y0= b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));

        cout<<pt2.x<<endl;
        line(roi_img,pt1,pt2,Scalar(255,0,255),1,LINE_AA);
//        circle(src_img,pt1,1,Scalar(255,0,255),-1,8);
//        circle(roi_img,pt2,1,Scalar(255,0,255),-1,8);
    }

    imshow("roi",roi_img);
    imshow("roi_img1",roi_img1);
    imshow("src_img",src_img);

    waitKey(0);
    return 0;
}

void on_callback(int,void *)
{
    threshold(g_gray_img,g_bin_img,g_thresh,g_max,g_type);
    imshow("g_bin_img",g_bin_img);

}


