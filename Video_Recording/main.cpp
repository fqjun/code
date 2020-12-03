#include <iostream>
#include "opencv2/highgui/highgui.hpp"  // 窗口显示
#include "opencv2/core/core.hpp"        // 数学运算

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    // 设置参数
    int cameraIndex = 0;
    double frameWidth = 1280;
    double frameHeight = 800;
    double fps = 10;

    // 初始化摄像头
    VideoCapture camera;
    camera.open(0);            // 去除黑边
    camera.set(CAP_PROP_FRAME_WIDTH, frameWidth);       // 宽度
    camera.set(CAP_PROP_FRAME_HEIGHT, frameHeight);     // 高度
    camera.set(CAP_PROP_FPS, fps);                      // 帧率

    // 初始化录像机
    VideoWriter recorder;
    frameWidth = camera.get(CAP_PROP_FRAME_WIDTH);      // 尺寸设置会失败, 使用get查询实际参数
    frameHeight = camera.get(CAP_PROP_FRAME_HEIGHT);
    cv::Size size(/*(int)frameWidth, (int)frameHeight*/1280,800);
    fps = camera.get(CAP_PROP_FPS);
    int fourcc = recorder.fourcc('M', 'J', 'P', 'G');   // 设置avi文件对应的编码格式
    recorder.open("/home/jun/workplace/录像/2020-10.avi", fourcc,30, size, CAP_DSHOW);
    if(recorder.isOpened())
    {
        cout << "正在录制...\n" << endl;
    }
    else
    {
        cerr << "参数错误, 录制失败.\n" << endl;
        return -1;
    }

    // 实时显示并录制
    for(int counter = 0;;)

    {
        cv::Mat frame;
        camera >> frame;
        cv::imshow("camera", frame);
        recorder << frame;
        char c =(char)waitKey(1);
        switch (c)
        {
        case 'a':
        imwrite("/home/jun/图片/工业相机/image.png",frame);   

        }
        printf("正在录制第%4d帧.\n", counter++);
        if(cv::waitKey(33) == 27) break;   // ESC 键退出
    }

    // 释放
    camera.release();     // 不必要
    recorder.release();   // 必要
    cv::destroyWindow("camera");
    cout << "\n录制完成." << endl;
    return 0;
}
