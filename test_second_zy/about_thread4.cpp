#include "about_thread4.h"

buffer_image buffer_img;
buffer_signal buffer_sig;
thread_administrator thread_admin;
vector<image_process_administrator> ip_administrators;
image_capture_administrator ic_administrator;
serialport_communicate_administrator sc_administrators;

VideoCapture capture(0);

//static int _count = 0;

///////////////////////////////////////////////////////////////////////
/*数据缓冲区管理类（包含各种共享数据和方法）*/
buffer::buffer()
{
}

buffer::buffer(unsigned long thread_quantity)
{
    init(thread_quantity);
}

void buffer::init(unsigned long thread_quantity)
{
    fake_semaphore = thread_quantity;
    counter = fake_semaphore;

    pthread_mutex_init (&mutex_message,nullptr);
    pthread_mutex_init (&mutex_cond,nullptr);


    pthread_cond_init(&cond,nullptr);
}
/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief 为每个下级线程（消费者）分发信号量
/// \return 一个信号量的指针（使用指针以确保线程与缓冲区之间使用的是同一个信号量
sem_t* buffer::get_sem()
{
    for(int i=0;i<8;++i)
    {
        if(sems.used_sem[i])
        {
            continue;
        }
        else
        {
            sems.used_sem[i] = true;
            sem_init(&sems.sequence[i],0,0);
            return &sems.sequence[i];
        }
    }
    return nullptr;
}

buffer_image::buffer_image()
{
}

buffer_image::buffer_image(unsigned long thread_quantity):buffer(thread_quantity)
{
    for(int i =0; i<8; ++i)
    {
        sems.used_sem[i] = false;
    }

}

void buffer_image::load_message(Mat input_img)
{
    image = input_img;
}

void buffer_image::clear_message(void)
{
    image = Mat::zeros(480, 640,CV_32F);
}

Mat buffer_image::get_message(void)
{
    pthread_mutex_lock(&mutex_message);
    counter--;

    if (counter == 0)
    {
        counter = fake_semaphore;
        pthread_mutex_lock(&mutex_cond);
        pthread_cond_signal(&cond);
//        _count++;
//        cout<<"thread: "<<_count<<endl;
        pthread_mutex_unlock(&mutex_cond);
    }
    Mat temp_img = image;
    pthread_mutex_unlock(&mutex_message);

    return temp_img;
}

buffer_signal::buffer_signal()
{
}

buffer_signal::buffer_signal(unsigned long thread_quantity)
{
    init(thread_quantity);
}
//buffer_signal::~buffer_signal()
//{
//    for(int i=0; i<MESSAGE_NUMBER; ++i)
//    {
//        delete passage[i];
//    }
//}
/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief 通信数据缓冲区初始化函数
/// \param 上一级（生产者）的数量（即图像处理线程的数量，以确定buffer维护的数据在一个周期中可以被使用几次）
void buffer_signal::init(unsigned long thread_quantity)
{
    fake_semaphore = thread_quantity;
    counter = fake_semaphore;
//    data.resize(thread_quantity);

    pthread_mutex_init (&mutex_message,nullptr);
    pthread_mutex_init (&mutex_cond,nullptr);

    pthread_cond_init(&cond,nullptr);

    clear_message();

//    for(int i=0; i<MESSAGE_NUMBER; ++i)
//    {
//        passage[i] = new char(35);
//    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief 用于获取图像处理线程返回的数据
/// \param 线程编号，用于确定返回的数据在二维数组中的位置（某一行）
/// \return 返回储存数据的二维数组的某一行首地址的形式，在图像处理函数中被更改以获取数据
///
char* buffer_signal::load_buffer(unsigned long thread_number)
{

    return &data[thread_number][0];//返回了某一行的数组首地址

}
void buffer_signal::load_message(void)
{
//    pthread_mutex_lock(&mutex_message);

//    counter--;
//    if (counter == 0)
//    {
//        counter = fake_semaphore;
//        pthread_mutex_lock(&mutex_cond);
//        pthread_cond_signal(&cond);
//        pthread_mutex_unlock(&mutex_cond);
//    }

//    if(temp_data)
//    {
//        vector<char> temp_char_arr(temp_data,temp_data+sizeof(temp_data)/sizeof(temp_data[0]));
//        for(unsigned long i=0;i<data.size();++i)
//        {
//            data.push_back(temp_char_arr);
//        }
//    }

    for(int i=1; i<MESSAGE_NUMBER; ++i)
    {
        if(data[i][0] != '\0')
        {
            for(int j=0; j<MESSAGE_NUMBER; ++j)
            {
                if(data[j][0] == '\0')
                {
                    strcpy(data[j], data[i]);
                    data[i][0] = '\0';
                    break;
                }
            }
        }
    }
    memcpy(data_buffer, data, sizeof(data));

//    int counter = 0;
//    for(int i=0; i<MESSAGE_NUMBER; ++i)
//    {
//        if(passage[i][0])
//        {
//            strcpy(data[counter], passage[i]);
//        }
//    }
//    pthread_mutex_unlock(&mutex_message);

}
/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief 清空所有储存通信数据的数组
void buffer_signal::clear_message(void)
{
    for(int i=0; i<MESSAGE_NUMBER; ++i)
    {
        //passage[i] = nullptr;
        if(data[i][0]!='\0')
        {
            data[i][0] = '\0';
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief 用于获取所有通信数据
/// \return 一个二维数组，每一行即一个图像处理线程返回的信息
signal_seq& buffer_signal::get_message(void)
{
//    pthread_cond_broadcast(&cond);
//    vector<vector<char>> temp_strs = data;

//    uint8_t set_counter = 0;
//    for(int i=0; i<MESSAGE_NUMBER; ++i)
//    {
//        if(passage[i])
//        {
//            strcpy(data[set_counter], passage[i]);
//            set_counter++;
//        }
//    }
    return data_buffer;
}
///////////////////////////////////////////////////////////////////////////////////////////////

thread_administrator::thread_administrator(void)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief 分发线程ID
pthread_t* thread_administrator::align(void)
{
    for(int i=0;i<8;++i)
    {
        if(thred_seq.used[i])
        {
            continue;
        }
        else
        {
            thred_seq.used[i] = true;
            return &thred_seq.thred_sequence[i];
        }
    }
    return nullptr;
}


///////////////////////////////////////////////////////////////////////////////////////////////

image_capture_administrator::image_capture_administrator()
{
}

image_capture_administrator::image_capture_administrator(Mat(*input_function)(void),
                                                         thread_administrator& input_thread_admin,
                                                         buffer_image& input_buffer)
{
    admin_init(input_function,input_thread_admin,input_buffer);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief 图像获取线程管理类初始化函数
/// \param Mat(*input_function)(void)类型函数指针（函数名）
///        thread_administrator（线程总管理类）
///        buffer_image（图像缓冲区类）
///        buffer_signal（通信数据缓冲区类）
void image_capture_administrator::admin_init(Mat(*input_function)(void),
                                             thread_administrator& input_thread_admin,
                                             buffer_image& input_bi)
{
    //连接线程管理类
    thread_admin = &input_thread_admin;
    //连接诶buffer管理类
    bi = &input_bi;

    //获取主处理函数
    function = input_function;

    //向线程管理类申请线程ID
    thread_id = thread_admin->align();

    serial_number = thread_admin->temp_serial_number;
    thread_admin->temp_serial_number++;
}


///////////////////////////////////////////////////////////////////////////////////////////////
image_process_administrator::image_process_administrator()
{
}

image_process_administrator::image_process_administrator(void(*input_function)(const Mat&, char*),
                                                         thread_administrator& input_thread_admin,
                                                         buffer_image& input_bi,
                                                         buffer_signal& input_bs)
{
    admin_init(input_function,input_thread_admin,input_bi,input_bs);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief 图像处理线程管理类初始化函数
/// \param void(*)(const Mat&, char*)类型函数指针（函数名）
///        thread_administrator（线程总管理类）
///        buffer_image（图像缓冲区类）
///        buffer_signal（通信数据缓冲区类）
void image_process_administrator::admin_init(void(*input_function)(const Mat&, char*),
                                             thread_administrator& input_thread_admin,
                                             buffer_image& input_bi,
                                             buffer_signal& input_bs)
{
    //连接线程管理类
    thread_admin = &input_thread_admin;
    //连接buffer管理类
    bi = &input_bi;
    bs = &input_bs;

    //获取主处理函数
    function = input_function;

    //向线程管理类申请线程ID
    thread_id = thread_admin->align();
    //向buffer管理类申请信号量
    sem = bi->get_sem();

    serial_number = thread_admin->temp_serial_number;
    thread_admin->temp_serial_number++;
}
///////////////////////////////////////////////////////////////////////////////////////////////
serialport_communicate_administrator::serialport_communicate_administrator()
{
}

serialport_communicate_administrator::serialport_communicate_administrator(//void(*input_function)(const vector<vector<char>>&),
                                                                           void(*input_function)(const signal_seq&),
                                                                           thread_administrator& input_thread_admin,
                                                                           buffer_signal& input_bs)
{
    admin_init(input_function,input_thread_admin,input_bs);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief 串口通信线程管理类初始化函数
/// \param void(*)(const signal_seq&)类型函数指针（函数名）
///        thread_administrator（线程总管理类）
///        buffer_signal（通信数据缓冲区类）
void serialport_communicate_administrator::admin_init(//void(*input_function)(const vector<vector<char>>&),
                                                      void(*input_function)(const signal_seq&),
                                                      thread_administrator& input_thread_admin,
                                                      buffer_signal& input_bs)
{
    //连接线程管理类
    thread_admin = &input_thread_admin;
    //连接buffer管理类
    bs = &input_bs;

    //获取主处理函数
    function = input_function;

    //向线程管理类申请线程ID
    thread_id = thread_admin->align();
    //向buffer管理类申请信号量
    sem = bs->get_sem();

    serial_number = thread_admin->temp_serial_number;
    thread_admin->temp_serial_number++;
}
/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief 图像获取线程运行的函数
/// \param 图像获取线程管理类
/// \return 任意类型的指针（弃用）
void* function_img_capture_completion(void* input_ica)
{
    image_capture_administrator *ica = static_cast<image_capture_administrator*>(input_ica);
    Mat(*function)(void) = ica->function;
    buffer_image* bi = ica->bi;
    pthread_cond_t* cond = &bi->cond;
    pthread_mutex_t* mutex_cond = &bi->mutex_cond;

    int size = sizeof(bi->sems.sequence)/sizeof(sem_t);
    int number_of_used_sem = 0;
    for(int i=0; i<size; ++i)
    {
        if(bi->sems.used_sem[i])
        {
            number_of_used_sem++;
        }
    }

    for (;;)
    {
        bi->load_message(function());
        pthread_mutex_lock(mutex_cond);
        for(int j=0; j<number_of_used_sem; ++j)
        {
            sem_post(&bi->sems.sequence[j]);
        }
        pthread_cond_wait(cond, mutex_cond);
        pthread_mutex_unlock(mutex_cond);
    }

}
/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief 图像处理线程运行的函数
/// \param 图像处理线程管理器
/// \return 任意类型的指针（弃用）
void* function_img_process_completion(void* input_ipa)
{
    image_process_administrator *ipa = static_cast<image_process_administrator*>(input_ipa);
    void(*function)(const Mat&, char*) = ipa->function;
    buffer_image* bi = ipa->bi;
    buffer_signal* bs = ipa->bs;

//    pthread_mutex_t* mutex_cond = &bs->mutex_cond;
    pthread_mutex_t* mutex_message = &bs->mutex_message;
    unsigned long* counter = &bs->counter;
    unsigned long fake_semaphore = bs->fake_semaphore;

    sem_t* sem_img = ipa->sem;
    unsigned long temp_serial_number = ipa->serial_number;

//    bool temp_flag = false;
    for (;;)
    {
        //信号量用于等待图像处理线程（生产者）传递信息
        sem_wait(sem_img);

        function(bi->get_message(),bs->load_buffer(temp_serial_number));
//        test = bs->load_buffer(temp_serial_number);
//        function(bi->get_message(),test);
        pthread_mutex_lock(mutex_message);
        (*counter)--;
        if (*counter == 0)
        {
            *counter = fake_semaphore;
            bs->load_message();
            sem_post(&bs->sems.sequence[0]);
            bs->clear_message();
//            temp_flag = true;
        }
        pthread_mutex_unlock(mutex_message);

//        if(!temp_flag)
//        {
//            pthread_mutex_lock(&mutex_cond);
//            pthread_cond_wait(cond, &mutex_cond);
//            pthread_mutex_unlock(&mutex_cond);
//            temp_flag = false;
//        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief 串口通信线程运行的函数
/// \param 串口通信线程管理类
/// \return 任意类型的指针（弃用）
void* function_serialport_communication_completion(void* input_sca)
{
    serialport_communicate_administrator *sca = static_cast<serialport_communicate_administrator*>(input_sca);
    void(*function)(const signal_seq&) = sca->function;
    buffer_signal* bs = sca->bs;
    sem_t* sem = sca->sem;
    for (;;)
    {
        //信号量用于等待图像处理线程（生产者）传递信息
        sem_wait(sem);
        function(bs->get_message());
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief 创建图像获取线程
/// \param image_capture_administrator（图像获取线程管理器）
void thread_creator_img_capture(image_capture_administrator *ica)
{
   pthread_create(ica->thread_id, nullptr, function_img_capture_completion, ica);
}
/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief 创建图像处理线程
/// \param image_process_administrator（图像处理线程管理器）
 void thread_creator_img_process(image_process_administrator *ipa)
 {
    pthread_create(ipa->thread_id, nullptr, function_img_process_completion, ipa);
 }
 /////////////////////////////////////////////////////////////////////////////////////////////
 /// \brief 创建串口通信线程
 /// \param serialport_communicate_administrator（串口通信线程管理器）
 ///
 void thread_creator_serialport_communication(serialport_communicate_administrator *sca)
 {
     pthread_create(sca->thread_id, nullptr, function_serialport_communication_completion, sca);
 }
 //////////////////////////////////////////////////////////////////////////////////////////////
 /// \brief 创建线程
 /// \param void(*)const signal_seq&类型的函数指针（函数名）
 ///        void(*)(const Mat&, char*)类型的函数指针（函数名）向量
 ///        Mat(*)(void))类型的函数指针（函数名），缺省为函数image_capture
 ///\attention 初始化各类型线程的顺序不可更改，必须是程序流程的倒序
void thread_creator(//void(*sep_commun_function)(const vector<vector<char>>&),
                    void(*sep_commun_function)(const signal_seq&),
                    vector<void(*)(const Mat&, char*)> img_process_functions,
                    Mat(*img_capture_function)(void))
{
    //初始化图片缓冲区，信息缓冲区
    buffer_img.init(img_process_functions.size());
    buffer_sig.init(img_process_functions.size());

    //初始化串口通信线程管理类，创建串口通信线程
    sc_administrators.admin_init(sep_commun_function,thread_admin,buffer_sig);
    thread_creator_serialport_communication(&sc_administrators);

    //依次初始化图像处理线程管理类，创建图像处理线程
    image_process_administrator temp_ipa;
    for (unsigned long i=0;i<img_process_functions.size();++i)
    {
        temp_ipa.admin_init(img_process_functions[i], thread_admin, buffer_img, buffer_sig);
        ip_administrators.push_back(temp_ipa);
        thread_creator_img_process(&ip_administrators[i]);
    }

    //初始化图像获取线程管理类，创建图像获取线线程
    ic_administrator.admin_init(img_capture_function, thread_admin, buffer_img);
    thread_creator_img_capture(&ic_administrator);

}

Mat image_capture(void)
{
    Mat src_img;
    capture >> src_img;
    return  src_img;
}
//8392704

