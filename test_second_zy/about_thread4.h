#ifndef ABOUT_THREAD_H
#define ABOUT_THREAD_H4

#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;
using namespace cv;

#define HIGHT 480
#define WIDTH 640

#define MESSAGE_NUMBER 6
#define MESSAGE_LONGTH 35
typedef  char signal_seq[MESSAGE_NUMBER][MESSAGE_LONGTH];


typedef struct thred_sequence
{
//    pthread_t thred_sequence[8] = {0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff};
    pthread_t thred_sequence[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    bool used[8]{false, false, false, false, false, false, false, false};
}thred_sequence;


typedef struct sem_sequence
{
    sem_t sequence[8];
    bool used_sem[8]{false, false, false, false, false, false, false, false};
}sem_sequence;


//管理缓冲区数据
class buffer
{
    public:
        buffer();
        buffer(unsigned long thread_quantity);//初始化作用线程数量

        unsigned long fake_semaphore;//该数据可收/发次数n
        unsigned long counter;//计数器，记录图像剩余可收/发次数

        pthread_mutex_t mutex_message;//读写时锁住数据的互斥锁
        pthread_mutex_t mutex_cond;//配合cond用的互斥锁
        sem_sequence sems;//定义控制n个线程的信号量
        pthread_cond_t cond = PTHREAD_COND_INITIALIZER;//用于所有线程完成任务时删除数据的条件变量(可能不需要)

        void init(unsigned long thread_quantity);
        sem_t* get_sem(void);//用于线程获取信号量
};

//管理图像数据，连接获取图像线程及图像处理线程
class buffer_image:public buffer
{
    public:
        buffer_image();
        buffer_image(unsigned long thread_quantity);

        void load_message(Mat input_img);//填充缓存区数据
        void clear_message(void);//清空缓存区数据
        Mat get_message(void);//获取数据图像，可用次数用完时发送信号（条件变量）给读图线程刷新图像缓存

        Mat image = Mat::zeros(480, 640,CV_32F);

};

class buffer_signal:public buffer
{
    public:
        buffer_signal();
        buffer_signal(unsigned long thread_quantity);
        //~buffer_signal();

        void init(unsigned long thread_quantity);
        char* load_buffer(unsigned long thread_number);//填充缓存的缓存
        void load_message(void);//填充缓存区数据
        void clear_message(void);//清空缓存区数据
        signal_seq& get_message(void);//获取数据图像，可用次数用完时发送信号（条件变量）给读图线程刷新图像缓存
//        vector<vector<char>>& get_message(void)
//        const char* passage[MESSAGE_NUMBER] ;//buffer的buffer
        signal_seq data;
        signal_seq data_buffer;
//        vector<vector<char>> data;
};

//管理线程，主要是（暂时只有）线程代号分配
class thread_administrator
{
    public:
        thread_administrator();
        thred_sequence thred_seq;
        unsigned long temp_serial_number = 0;
        pthread_t* align(void);
    private:
};

//获取图像线程管理器
class image_capture_administrator
{
    public:
        image_capture_administrator();
        image_capture_administrator(Mat(*input_function)(void),
                                    thread_administrator& input_thread_admin,
                                    buffer_image& input_buffer);
        void admin_init(Mat(*input_function)(void),
                        thread_administrator& input_thread_admin,
                        buffer_image& input_buffer);

        pthread_t* thread_id;
        Mat(*function)(void);
        thread_administrator* thread_admin;
        buffer_image* bi;

        unsigned long serial_number;
};

//图像处理线程管理器
class image_process_administrator
{
    public:
        image_process_administrator();
        image_process_administrator(void(*input_function)(const Mat&, char*),
                                    thread_administrator& input_thread_admin,
                                    buffer_image& input_bi,
                                    buffer_signal& input_bs);
        void admin_init(void(*input_function)(const Mat&, char*),
                        thread_administrator& input_thread_admin,
                        buffer_image& input_bi,
                        buffer_signal& input_bs);

        pthread_t* thread_id;
        sem_t* sem;
        void(*function)(const Mat&, char*);
        thread_administrator* thread_admin;
        buffer_image* bi;
        buffer_signal* bs;

        unsigned long serial_number;
};

//串口通信线程管理器
class serialport_communicate_administrator
{
    public:
        serialport_communicate_administrator();
        serialport_communicate_administrator(//void(*input_function)(const vector<vector<char>>&),
                                             void(*input_function)(const signal_seq&),
                                             thread_administrator& input_thread_admin,
                                             buffer_signal& input_bs);
        void admin_init(//void(*input_function)(const vector<vector<char>>&),
                        void(*input_function)(const signal_seq&),
                        thread_administrator& input_thread_admin,
                        buffer_signal& input_bs);

        pthread_t* thread_id;
        sem_t* sem;
//        void(*function)(const vector<vector<char>>&);
        void(*function)(const signal_seq&);
        thread_administrator* thread_admin;
        buffer_signal* bs;

        unsigned long serial_number;
};

Mat image_capture(void);


void* function_img_capture_completion(void* input_images);
void thread_creator_img_capture(pthread_t id, image_capture_administrator *ica);

void* function_img_process_completion(void* input_images);
void thread_creator_img_process(pthread_t id, image_process_administrator *ipa);

void* function_serialport_communication_completion(vector<char*> input_ipa);
void thread_creator_serialport_communication(serialport_communicate_administrator *sca);

void thread_creator(//void(*sep_commun_function)(const vector<vector<char>>&),
                    void(*input_function)(const signal_seq&),
                    vector<void(*)(const Mat&, char*)> functions,
                    Mat(*img_capture_function)(void)=image_capture);

extern buffer_image buffer_img;
extern buffer_signal buffer_sig;
extern thread_administrator thread_admin;
extern vector<image_process_administrator> ip_administrators;
extern image_capture_administrator ic_administrator;
extern serialport_communicate_administrator sc_administrators;
extern VideoCapture capture;
#endif // ABOUT_THREAD_H4
