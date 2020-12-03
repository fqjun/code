#include "about_thread4.h"
#include <stdio.h>
void normal_process(const Mat& input_img, char* output_data);
void contrast_process(const Mat& input_img, char* output_data);
void serialportg_communication(const signal_seq& input_data);//static int _count = 0;
//static int sem_count = 0;
static int mark_count1 = 0;
static int mark_count2 = 0;
//static double t_start;
//static double t_result;

int main()
 {
    Mat src_img;
    vector<char[50]> strs(2);
    vector<void(*)(const Mat&, char*)> functions;
    functions.push_back(normal_process);
    functions.push_back(contrast_process);

    thread_creator(serialportg_communication,functions);
    //t_start = static_cast<double>(getTickCount());
    for (;;)
    {
//        if(!getchar())
//        {
//            break;
//        }
    }
    return 0;
}

void normal_process(const Mat& input_img, char* output_data)
{
//    cout<<"normal_process"<<endl;
    Mat origin_img = input_img;
    if (origin_img.data)
    {
        inRange(origin_img,  Scalar(90,125,125),Scalar(180,255,255), origin_img);
        Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
        morphologyEx(origin_img, origin_img, MORPH_OPEN, element);
//        imshow("normal test", origin_img);
//        waitKey(1);
        mark_count1++;
        cout<<"mark1:"<<mark_count1<<endl;
    }
    else
    {
        cout<<"not image"<<endl;
    }
    char test[11];
    sprintf(test,"%c%d%c%03d%c%03d", '$', 1, ',', WIDTH/2, ',', HIGHT/2);
    strcpy(output_data, test);

}

void contrast_process(const Mat& input_img, char* output_data)
{
//    cout<<"contrast_process"<<endl;
    Mat origin_img = input_img;
    if (origin_img.data)
    {
        cvtColor(origin_img, origin_img, COLOR_BGR2HSV);
//        imshow("contrast test", origin_img);
//        waitKey(1);
        mark_count2++;
        cout<<"mark2:"<<mark_count2<<endl;
    }
    else
    {
        cout<<"not image"<<endl;
    }
    char test[11];
    sprintf(test,"%c%d%c%03d%c%03d", '$', 2, ',', WIDTH/2, ',', HIGHT/2);
    strcpy(output_data, test);
}

void serialportg_communication(const signal_seq& input_data)
{
//    double t_now = getTickCount();
//    t_result = static_cast<double>((t_now - t_start)/getTickFrequency());
//    cout<<_count++<<": "<<t_result<<endl;
//    t_start = t_now;
    char temp_str[MESSAGE_LONGTH];

    for(unsigned long i=0;i<MESSAGE_NUMBER;++i)
    {
        if(strlen(input_data[i]))
        {
            strcpy(temp_str, input_data[i]);
//            printf("%s\n",temp_str);
        }
    }
//    cout<<endl;
//    cout<<"serialportg_communication"<<endl;
}

