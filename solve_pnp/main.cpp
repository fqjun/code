#include <iostream>
#include "CameraApi.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include "solve_pnp.h"
using namespace std;
using namespace cv;

int main()
{
    VideoCapture cap(1);
    unsigned char *g_pRgbBuffer; //处理后数据缓存区

    int                     iCameraCounts = 1;
    int                     iStatus=-1;
    tSdkCameraDevInfo       tCameraEnumList;
    int                     hCamera;
    tSdkCameraCapbility     tCapability;      //设备描述信息
    tSdkFrameHead           sFrameInfo;
    BYTE*			        pbyBuffer;
    IplImage                *iplImage = NULL;
    int                     channel=3;
    BOOL                    AEstate=FALSE;

    CameraSdkInit(1);
    //枚举设备，并建立设备列表
    iStatus = CameraEnumerateDevice(&tCameraEnumList,&iCameraCounts);
    printf("state = %d\n", iStatus);
    printf("count = %d\n", iCameraCounts);
    //没有连接设备
    if(iCameraCounts==0)
    {
        return -1;
    }
    //相机初始化。初始化成功后，才能调用任何其他相机相关的操作接口
    iStatus = CameraInit(&tCameraEnumList,-1,-1,&hCamera);
    //初始化失败
    printf("state = %d\n", iStatus);
    if(iStatus!=CAMERA_STATUS_SUCCESS)
    {
        return -1;
    }
    //获得相机的特性描述结构体。该结构体中包含了相机可设置的各种参数的范围信息。决定了相关函数的参数
    CameraGetCapability(hCamera,&tCapability);
    g_pRgbBuffer = (unsigned char*)malloc(tCapability.sResolutionRange.iHeightMax*tCapability.sResolutionRange.iWidthMax*3);
    cout<<CameraGetAeState(hCamera,&AEstate);
    cout<<CameraSetAeState(hCamera,FALSE);

    CameraSetExposureTime(hCamera,30000);
    /*让SDK进入工作模式，开始接收来自相机发送的图像数据。
     *如果当前相机是触发模式，则需要接收到触发帧以后才会更新图像*/
    CameraPlay(hCamera);
    /*
    其他的相机参数设置
    例如 CameraSetExposureTime   CameraGetExposureTime  设置/读取曝光时间
         CameraSetImageResolution  CameraGetImageResolution 设置/读取分辨率
         CameraSetGamma、CameraSetConrast、CameraSetGain等设置图像伽马、对比度、RGB数字增益等等。
         更多的参数的设置方法，，清参考MindVision_Demo。本例程只是为了演示如何将SDK中获取的图像，转成OpenCV的图像格式,以便调用OpenCV的图像处理函数进行后续开发
    */
    if(tCapability.sIspCapacity.bMonoSensor)
    {
        channel=1;
        CameraSetIspOutFormat(hCamera,CAMERA_MEDIA_TYPE_MONO8);
    }else
    {
        channel=3;
        CameraSetIspOutFormat(hCamera,CAMERA_MEDIA_TYPE_BGR8);
    }

    RM_SolveAngle Pnp;
    while(true)
    {
        if(CameraGetImageBuffer(hCamera,&sFrameInfo,&pbyBuffer,1000) == CAMERA_STATUS_SUCCESS)
        {

            CameraImageProcess(hCamera, pbyBuffer, g_pRgbBuffer,&sFrameInfo);
            if (iplImage)
            {
                cvReleaseImageHeader(&iplImage);
            }
            iplImage = cvCreateImageHeader(cvSize(sFrameInfo.iWidth,sFrameInfo.iHeight),IPL_DEPTH_8U,channel);
            cvSetData(iplImage,g_pRgbBuffer,sFrameInfo.iWidth*channel);//此处只是设置指针，无图像块数据拷贝，不需担心转换效率

            //Mat Iimag(iplImage);//这里只是进行指针转换，将IplImage转换成Mat类型
            Mat src_img = cvarrToMat(iplImage,true);
            Mat frame;
            Mat inputImg;
            Mat hsv_red;
            Mat src1;
            Mat src2;
            resize(src_img, src_img, Size(640, 480));
            src_img.copyTo(frame);
            src_img.copyTo(inputImg);
            //imshow("OpenCV Demo",src_img);
            blur(src_img, src_img, Size(3, 3));
            cvtColor(src_img, src_img, COLOR_BGR2HSV);

            inRange(src_img, Scalar(156, 100, 30), Scalar(180, 255, 255), src1);
            inRange(src_img, Scalar(0, 120, 20), Scalar(10, 255, 255), src2);
            hsv_red = src1 + src2;
            imshow("output", hsv_red);

            vector < vector < Point >> contours_1;
            vector < Vec4i > hierarchy_1;
            findContours(hsv_red, contours_1, hierarchy_1, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));
            RotatedRect surround_rect;
            Point2f vertex[4];

            for (int i1 = 0; i1 < (int) contours_1.size(); ++i1){
                surround_rect = minAreaRect(contours_1[i1]);
                double area = surround_rect.size.width * surround_rect.size.height;

                if(area > 3000){

                    double w = surround_rect.size.width;
                    double h = surround_rect.size.height;
                    double radio;
                    if (h >= w) {
                        double high = w;
                        double width = h;
                        radio = width / high;
                    } else {
                        double high = h;
                        double width = w;
                        radio = width / high;
                    }
                    //cout << radio << endl;


                    if(radio >= 1.08 && radio < 1.96){
                        surround_rect.points(vertex);
                        for(int i2 = 0; i2 < 4; i2++){
                            line(frame, vertex[i2], vertex[(i2 + 1) % 4], Scalar(255, 100, 200),2,CV_AA);
                        }
                        Pnp.run_SolvePnp(frame, surround_rect);
                    }
                }
            }

            imshow("last_img", frame);

            //在成功调用CameraGetImageBuffer后，必须调用CameraReleaseImageBuffer来释放获得的buffer。
            //否则再次调用CameraGetImageBuffer时，程序将被挂起一直阻塞，直到其他线程中调用CameraReleaseImageBuffer来释放了buffer
            CameraReleaseImageBuffer(hCamera,pbyBuffer);
            char k  = waitKey(1);
            if(k == 27)
            {
                break;
            }

        }
    }

    CameraUnInit(hCamera);
    //注意，现反初始化后再free
    free(g_pRgbBuffer);

    return 0;
}
