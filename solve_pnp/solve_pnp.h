#ifndef SOLVE_PNP_H
#define SOLVE_PNP_H
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace std;
using namespace cv;

class RM_SolveAngle{
public:
    RM_SolveAngle();
    ~RM_SolveAngle();
    //调用解算函数
    void run_SolvePnp(Mat & srcImg, RotatedRect & rect);

    float angle_x, angle_y, dist;

private:
    void vertex_Sort(RotatedRect & box);
    Mat camera_ptz(Mat & t);
    void get_Angle(const Mat & pos_in_ptz);
    void draw_Coordinate(Mat & input);
    //标定数据
    //string file_path = "/home/hzh/文档/cameraParams_infantry4.xml";
    string file_path = "/home/jun/桌面/solve_pnp/cameraParams_infantry4.xml";
    Mat cameraMatrix, distCoeffs;
    Mat rvec = Mat::zeros(3, 3, CV_64FC1);
    Mat tvec = Mat::zeros(3, 1, CV_64FC1);

    vector<Point3f> armor3d;
    vector<Point2f> target2d;

    const float ptz_camera_x = 0;
    const float ptz_camera_y = 0;
    const float ptz_camera_z = 0;
    const float barrel_ptz_offset_x = 0;
    const float barrel_ptz_offset_y = 0;
    const float overlap_dist = 0;
    //装甲板单位mm
    float Armor_W = 70;
    float Armor_H = 50;
    float half_x = Armor_W / 2;
    float half_y = Armor_H / 2;
};


#endif // SOLVE_PNP_H
