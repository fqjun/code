#include "about_thread2.h"
static int _count = 0;

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

    for(unsigned long i=0;i<fake_semaphore;++i)
    {
        sem_init(&sems.sem_sequence[i],0,0);
    }

    pthread_cond_init(&cond,nullptr);
}

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
            return &sems.sem_sequence[i];
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
    image = &input_img;
}

void buffer_image::clear_message(void)
{
    image = nullptr;
}

Mat* buffer_image::read_message(void)
{
    counter--;

    if (counter == 0)
    {
        counter = fake_semaphore;
        pthread_mutex_lock(&mutex_cond);
        pthread_cond_signal(&cond);
        _count++;
        cout<<"thread: "<<_count<<endl;
        pthread_mutex_unlock(&mutex_cond);
    }
    return image;
}

buffer_signal::buffer_signal()
{
}

buffer_signal::buffer_signal(unsigned long thread_quantity):buffer(thread_quantity)
{
}
void buffer_signal::load_message(char* input_data)
{
    data = input_data;
}

void buffer_signal::clear_message(void)
{
    data = nullptr;
}

char* buffer_signal::read_message(void)
{
    counter--;
    if (counter == 0)
    {
        counter = fake_semaphore;
        pthread_mutex_lock(&mutex_cond);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex_cond);
    }
    return data;
}
///////////////////////////////////////////////////////////////////////////////////////////////

thread_administrator::thread_administrator(void)
{
}

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
            return &thred_seq.thred_sequence[i];
        }
    }
    return nullptr;
}
///////////////////////////////////////////////////////////////////////////////////////////////
image_process_administrator::image_process_administrator()
{
}

image_process_administrator::image_process_administrator(thread_administrator* input_thread_admin,
                                                         buffer_image* input_buffer,
                                                         void(*input_function)(void*))
{
    admin_init(input_thread_admin,input_buffer,input_function);
}

void image_process_administrator::admin_init(thread_administrator* input_thread_admin,
                                             buffer_image* input_buffer,
                                             void(*input_function)(void*))
{
    //连接线程管理类
    thread_admin = input_thread_admin;
    //连接诶buffer管理类
    buffer = input_buffer;

    //获取主处理函数
    function = input_function;

    //向线程管理类申请线程ID
    thread_id = thread_admin->align();
    //向buffer管理类申请信号量
    sem = buffer->get_sem();

}



///////////////////////////////////////////////////////////////////////////////////////////////

void* function_img_process_completion(void* input_images)
{
    image_process_administrator *images = static_cast<image_process_administrator*>(input_images);
    void(*function)(void*) = images->function;
    buffer_image* buffer = images->buffer;
    pthread_mutex_t* mutex = &buffer->mutex_message;
    //pthread_cond_t cond = buffer.cond;
    sem_t* sem = images->sem;
    for (;;)
    {
        sem_wait(sem);
        //可以把这把锁加在readmessage里
        pthread_mutex_lock(mutex);
        function(buffer->read_message());
        pthread_mutex_unlock(mutex);
    }

}


 void thread_creator_img_process(image_process_administrator *images)
 {
    pthread_create(images->thread_id, nullptr, function_img_process_completion, images);
 }



void thread_creator(vector<void(*)(void*)> functions)
{
    buffer_img.init(functions.size());
    image_process_administrator temp_admin;
    for (unsigned long i=0;i<functions.size();++i)
    {
        temp_admin.admin_init(&thread_admin, &buffer_img, functions[i]);
        administrators.push_back(temp_admin);
        thread_creator_img_process(&administrators[i]);
    }
}
