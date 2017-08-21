#include <iostream>
using namespace std;

/*  
void InsertSort(int *a,const int len)
{
    int i,j,tmp;
    for (i = 1; i < len; i++)
    {
        tmp = a[i];
        for (j = i-1; j>=0 && a[j]>tmp ;j-- )
            a[j+1] = a[j];
        a[j+1] = tmp;
    }
}
*/


void Insert(int tmp,int *a, int i)      //tmp是要插入的值，a是被插入的，i是a的長度
{
    int j;
    for (j = i-1; j>=0 && a[j]>tmp ;j-- )
        a[j+1] = a[j];
    a[j+1] = tmp;
}

void InsertSort(int *a,const int len)
{
    for (int i = 1; i < len; i++)       //從第二個開始依序插入
        Insert(a[i],a,i);
}

int main(int argc, char *argv[])
{
    int a[] = {54,3,4,2,34,1,4,124,15,54,12,3};
    int len = sizeof(a)/sizeof(int);
    cout << "陣列長度 " << len <<endl<<endl;
    cout << "排序前："<< endl;
    for (int i = 0; i < len; ++i) 
        cout << a[i] <<" ";

    InsertSort(a,len);

    cout << endl << endl;
    cout << "排序後："<< endl;
    for (int i = 0; i < len; ++i) 
        cout << a[i] <<" ";
    
    return 0;
}
