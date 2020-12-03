#ifndef ABOUT_THREAD_H2
#define ABOUT_THREAD_H2

#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;
using namespace cv;

#define thread_semaphore 0x03

typedef struct thred_sequence
{
    pthread_t thred_sequence[8] = {0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff};
    bool used[8]{false, false, false, false, false, false, false, false};
}thred_sequence;


typedef struct sem_sequence
{
    sem_t sem1, sem2, sem3, sem4, sem5, sem6, sem7, sem8;
    sem_t sem_sequence[8] = {sem1, sem2, sem3, sem4, sem5, sem6, sem7, sem8};
    bool used_sem[8]{false, false, false, false, false, false, false, false};
}sem_sequence;


//管理缓冲区数据
class buffer
{
    public:
        buffer();
        buffer(unsigned long thread_quantity);//初始化作用线程数量

        unsigned long fake_semaphore;//该数据可使用次数n
        unsigned long counter;//计数器，记录图像剩余可读取次数

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
        Mat* image;
        void load_message(Mat input_img);//填充缓存区数据
        void clear_message(void);//清空缓存区数据
        Mat* read_message(void);//获取数据图像，可用次数用完时发送信号（条件变量）给读图线程刷新图像缓存

};

class buffer_signal:public buffer
{
    public:
        buffer_signal();
        buffer_signal(unsigned long thread_quantity);
        char* data;
        void load_message(char* input_data);//填充缓存区数据
        void clear_message(void);//清空缓存区数据
        char* read_message(void);//获取数据图像，可用次数用完时发送信号（条件变量）给读图线程刷新图像缓存

};

//管理线程，主要是（暂时只有）线程代号分配
class thread_administrator
{
    public:
        thread_administrator();
        thred_sequence thred_seq;
        pthread_t* align(void);
    private:
};

//图像处理线程管理器
class image_process_administrator
{
    public:
        image_process_administrator();
        image_process_administrator(thread_administrator* input_thread_admin,
                                   buffer_image* input_buffer,
                                   void(*input_function)(void*));
        void admin_init(thread_administrator* input_thread_admin,
                        buffer_image* input_buffer,
                        void(*input_function)(void*));

        pthread_t* thread_id;
        sem_t* sem;
        void(*function)(void*);
        thread_administrator *thread_admin;
        buffer_image *buffer;


};



void thread_creator_img_process(pthread_t id, image_process_administrator *images);
void* function_img_process_completion(void* input_images);
void thread_creator(vector<void(*)(void*)> functions);

extern buffer_image buffer_img;
extern thread_administrator thread_admin;
extern vector<image_process_administrator> administrators;
#endif // ABOUT_THREAD_H2
