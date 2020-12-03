#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
//    ofstream file ("test.csv");
//    file<<format(mat,Formatter::FMT_CSV);
//    file.close();



    vector<vector<int> >vec;
    vector<int>value;
    vector<char>line;

    value.push_back(123);
    value.push_back(2);

//    Mat mat = Mat::ones(Size(2,value.size()),CV_8UC1);
    Mat mat ;
    mat.push_back(static_cast<Mat>(value));

    cout<<mat.size<<endl;

    for(auto i = 0;i<mat.rows;++i){

        uchar* data = mat.ptr<uchar>(i);

        for(auto j = 0;j<mat.cols;++j){

            cout<<"data["<<i<<"]["<<j<<"]="<<(int)data[j];

        }
        cout<<endl;
    }

//        for (int i=1;i<5;++i)
//        {
//            int b = 0;
//            cout<<"b="<<endl;
//            cin>>b;

//            for (int j=0;j<b;++j)
//            {
//                int a;
//                cout<<"a="<<endl;
//                cin>>a;
//                array1.push_back(a);
//            }
//            vec.push_back(array1);
//            array1.clear();

//        }

//        for(auto i = 0;i<vec.size();++i){
//            for(auto j =0;j<vec[i].size();++j){
//                cout<<"vec["<<i<<"]["<<j<<"] value = "<<vec[i][j]<<endl;
//            }
//            cout<<endl;
//        }
    return 0;
}
