#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    //声明传值和整合Mat以及vector
    Mat mat ;

    vector<float>test_float;
    vector<float>test;
//    vector<int>test1;

    Mat value;
    Mat line;
    Mat mat_value/* = Mat(3,1,CV_32F)*/;

//    cout<<"mat_value.size = "<<mat_value.size<<endl;
    //对mat和vector赋值
    test.push_back(2352.34);
    test.push_back(1);
    test.push_back(8882.34);

    test_float.push_back(4899.345345);
    test_float.push_back(13.231);
    test_float.push_back(5733.231);

    cout<<"test = "<<test.at(0)<<endl;
    cout<<"test = "<<test.at(1)<<endl;
    cout<<"test = "<<test.at(2)<<endl;

    cout<<"test_float = "<<test_float.at(0)<<endl;
    cout<<"test_float = "<<test_float.at(1)<<endl;
    cout<<"test_float = "<<test_float.at(2)<<endl;

    cout<<"test.size() = "<<test.size()<<endl;

    auto iter_test = test.begin();
    auto iter_test_float = test_float.begin();

    mat_value = Mat(test.size(),2,CV_32F);
    cout<<"mat_value.rows = "<<mat_value.rows<<endl;
    cout<<"mat_value.cols = "<<mat_value.cols<<endl;

    for(auto i = 0;i<mat_value.rows;++i){

        float* data = mat_value.ptr<float>(i);

        for(auto j = 0;j<mat_value.cols;++j){

            data[0] = *(iter_test + i);
            data[1] = *(iter_test_float + i);

            cout<<"data["<<i<<"]["<<j<<"]="<<(float)data[j]<<"  ";

//              cout<<"data["<<i<<"]["<<j<<"]="<<mat_value.at<float>(i,j)<<"  ";


        }
        cout<<endl;
    }



    //打开csv文件并传入数据
    ofstream file ("test.csv");
    file<<format(mat_value,Formatter::FMT_CSV);
    file.close();

    //打印mat里面的数据
    cout<<mat.size<<endl;

    for(auto i = 0;i<mat_value.rows;++i){

        float* data = mat_value.ptr<float>(i);

        for(auto j = 0;j<mat_value.cols;++j){

            cout<<"data["<<i<<"]["<<j<<"]="<<(float)data[j]<<"  ";

        }
        cout<<endl;
    }

    return 0;
}
