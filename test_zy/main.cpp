#include "about_thread2.h"

buffer_image buffer_img;
thread_administrator thread_admin;
vector<image_process_administrator> administrators;

void normal_process(void* input_img);
void contrast_process(void* input_img);
void sems_post(void);

static int _count = 0;
static int sem_count = 0;
static int mark_count1 = 0;
static int mark_count2 = 0;

int main()
 {
    VideoCapture capture(0);
    Mat src_img;

    vector<void(*)(void*)> functions;
    functions.push_back(normal_process);
    functions.push_back(contrast_process);

    thread_creator(functions);
    buffer_img.image = &src_img;

    sleep(1);
    //cout<<"ready"<<endl;
    for (;;)
    {
//        double t_mutex_messagestart = static_cast<double>(getTickCount());


        //cout<<"main"<<endl;
        capture >> src_img;
        pthread_mutex_lock(&buffer_img.mutex_cond);
        sems_post();
        //cout<<"test1"<<endl;
        //要保证pthread_cond_signal在wait之后运行
        //也即在执行可能触发signal的语句之前就要把保护wait的互斥锁关上。
        pthread_cond_wait(&buffer_img.cond, &buffer_img.mutex_cond);
        _count++;
        cout<<"main: "<<_count<<endl;
        pthread_mutex_unlock(&buffer_img.mutex_cond);

        //cout<<"test2"<<endl;


//        double t_result = (static_cast<double>(getTickCount()) - t_start)/getTickFrequency();
//        cout<<t_result<<endl;
    }
    return 0;
}

void normal_process(void* input_img)
{
    Mat* temp_img = static_cast<Mat*>(input_img);
    Mat origin_img = *temp_img;
    if (origin_img.data)
    {
        inRange(origin_img,  Scalar(90,125,125),Scalar(180,255,255), origin_img);
        Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
        morphologyEx(origin_img, origin_img, MORPH_OPEN, element);
        imshow("normal test", origin_img);
        waitKey(1);
        //        mark_count1++;
        //        cout<<"mark1:"<<mark_count1<<endl;
        cout<<"normal_process"<<endl;
    }
}

void contrast_process(void* input_img)
{

    Mat origin_img = *static_cast<Mat*>(input_img);
    if (origin_img.data)
    {
        cvtColor(origin_img, origin_img, COLOR_BGR2HSV);
        imshow("contrast test", origin_img);
        waitKey(1);
//        mark_count2++;
//        cout<<"mark2:"<<mark_count2<<endl;
        cout<<"contrast_process"<<endl;
    }
}

void sems_post(void)
{
    sem_count++;
    cout<<"sem_count: "<<sem_count<<endl;
//    int size = sizeof(buffer_img.sems.sem_sequence);
    for(int i=0; i<8; ++i)
    {
        if(buffer_img.sems.used_sem[i])
        {
            sem_post(&buffer_img.sems.sem_sequence[i]);
            cout<<"mark"<<endl;
        }
        else
        {
            break;
        }
    }
}
