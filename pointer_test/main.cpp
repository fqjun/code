#include <iostream>

using namespace std;
const int MAX = 4;

void changeValue(int *a);

int main()
{

    int aaa = 232;

    cout<<"aaa:"<<&aaa<<endl;
    changeValue(&aaa);
    cout<<"aaa:"<<&aaa<<endl;
    cout<<"value:"<<aaa<<endl;
    return 0;
}

void changeValue(int *a){
    *a = 11;
}
