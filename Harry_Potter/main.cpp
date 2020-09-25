/**
 * @brief 
 * @date 2020-08-16
 * @note ����Ȥ�����Լ����ϸ��±����Ĺ���
 */

// #define SET_COLOR //��Ҫ����Ļ����ɫʱȡ������ע��
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#ifdef SET_COLOR

void on_trackbar(int, void*);
int g_h_max=255, g_h_min=0;
int g_s_max=255, g_s_min=0;
int g_v_max=255, g_v_min=0;
int g_gray_max = 255;
#endif

Mat src_img, hsv_img, background_img;
Mat mask;

int main() {
	
	VideoCapture cap(3);
	if (!cap.isOpened()) {
		cout << "camera cna't open" << endl;
		return -1;
	}

	for (int i = 0; i < 30; ++i) {
		cap >> background_img;
	}

	while (1) {
		cap >> src_img;
		// imshow("src", src_img);
		cvtColor(src_img, hsv_img, COLOR_BGR2HSV);
		if(waitKey(1)==97){
			cap >> background_img;
			}

#ifdef SET_COLOR
		//h
		createTrackbar("max_h", "src", &g_h_max, 255, nullptr);
		createTrackbar("min_h", "src", &g_h_min, 255, nullptr);
		//s
		createTrackbar("max_s", "src", &g_s_max, 255, nullptr);
		createTrackbar("min_s", "src", &g_s_min, 255, nullptr);
		//v
		createTrackbar("max_v", "src", &g_v_max, 255, nullptr);
		createTrackbar("min_v", "src", &g_v_min, 255, nullptr);
		inRange(hsv_img, Scalar(g_h_min, g_s_min, g_v_min), Scalar(g_h_max, g_s_max, g_v_max), mask);//yellow 67,126,164    208,144,205  \  14,198,102     29,255,231
		

		imshow("mask", mask);
		// on_trackbar(0, 0);
#else
		Mat dst_img, stealth_img, show_img;
		Mat mask_1, mask_2,mask_3,mask_4;
	
		GaussianBlur(src_img,src_img,Size(5,5),1);

		inRange(hsv_img, Scalar(0,19,41), Scalar(6,214,133), mask_1);//    |body:12,121,136   255,149,170
		inRange(hsv_img, Scalar(164,34,41), Scalar(180,210,224), mask_2);
		inRange(hsv_img, Scalar(0,180,80 ), Scalar(15,255,255), mask_4);

		//orange:0,180,80   15,255,255
		//orange:0,180,30   15,255,255
		//pink:	0,19,41   6,214,133        164,34,41   180,210,224
		mask_1 = mask_1 + mask_2;
		mask_1 = mask_1 + mask_4;
		//body:67,126,164    208,144,205
		//26,122,136    111,143,163  129
		//10,117,14       35,255,125
    	// imshow("mask_1",mask_1);

		// imshow("mask_1",mask_1);
    	erode(mask_1,mask_1,getStructuringElement(MORPH_ELLIPSE,Size(11,11)));
		// erode(mask_1,mask_1,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
		medianBlur(mask_1,mask_1,5);

    	// imshow("mask_2",mask_1);

		dilate(mask_1, mask_1, getStructuringElement(MORPH_ELLIPSE, Size(9,9)));   
		dilate(mask_1, mask_1, getStructuringElement(MORPH_ELLIPSE, Size(9,9)));    
    	// imshow("mask_2",mask_1);


		// dilate(mask_1, mask_1, getStructuringElement(MORPH_RECT, Size(3,3)));    

		bitwise_not(mask_1, mask_3);//
		bitwise_and(src_img, src_img, show_img, mask_3);
		bitwise_and(background_img, background_img, stealth_img, mask_1);
		addWeighted(show_img, 1, stealth_img, 1, 0, dst_img);
		resize(dst_img,dst_img,Size(1080,1080));
		imshow("dst_img", dst_img);
#endif

		if (waitKey(1) == 27) {
			break;
		}
	}

	return 0;
}

#ifdef SET_COLOR
void on_trackbar(int, void*) {
	Mat mask;
	inRange(hsv_img, Scalar(g_h_min, g_s_min, g_v_min), Scalar(g_h_max, g_s_max, g_v_max), mask);
	imshow("mask", mask);
}
//test of git
#endif
