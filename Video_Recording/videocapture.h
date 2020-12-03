#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core.hpp>
#include <opencv2/video/tracking.hpp>
#include <math.h>
#include <fstream>
#include "CameraApi.h"
#include <fcntl.h>  //文件控制定义
#include <termios.h>   //POSIX终端控制定义
#include <unistd.h>    //UNIX标准定义
#include <errno.h>     //ERROR数字定义
#include <sys/select.h>
/*---工业相机中使用到opencv2.0的 IplImage 需要包含此头文件 ---*/
#include "opencv2/imgproc/imgproc_c.h"

using namespace std;
using namespace cv;

#define CAMERA_EXPOSURETIME 800
#define CAMERA_RESOLUTION_COLS 1280//1280
#define CAMERA_RESOLUTION_ROWS 800//800
#define CAMERA_RESOLUTION_COLS_FOV ((1280-CAMERA_RESOLUTION_COLS)*0.5)
#define CAMERA_RESOLUTION_ROWS_FOV ((1024-CAMERA_RESOLUTION_ROWS)*0.5)

class RM_VideoCapture
{
public:
    /***************************相机驱动自带*******************************/
    unsigned char *g_pRgbBuffer; //处理后数据缓存区

    int                     iCameraCounts = 1;
    int                     iStatus=-1;
    tSdkCameraDevInfo       tCameraEnumList;
    int                     hCamera;
    tSdkCameraCapbility     tCapability;      //设备描述信息
    tSdkFrameHead           sFrameInfo;
    BYTE*			        pbyBuffer;
    IplImage                *iplImage = nullptr;
    int                     channel=3;
    BOOL                    AEstate=FALSE;
    tSdkImageResolution     pImageResolution;       //相机分辨率信息
 /**********************************************************/
    bool                    iscamera0_open = false;

public:
    RM_VideoCapture(int cameramode);
    ~RM_VideoCapture();
    bool isindustryimgInput();
    void cameraReleasebuff();
    int cameraSet();

};


#endif // VIDEOCAPTURE_H
