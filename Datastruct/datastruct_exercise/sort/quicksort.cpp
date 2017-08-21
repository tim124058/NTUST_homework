#include <iostream>
using namespace std;

#define swap(x,y){int t;t=x;x=y;y=t;}


void QuickSort(int *a,const int left,const int right)
{
    if (left < right) {
        int i = left,j=right+1,pivot = a[left];
        do {
            do i++; while(a[i]<pivot);
            do j--; while(a[j]>pivot);
            if(i<j) swap(a[i],a[j]);
        } while(i<j);
        swap(a[left],a[j]);
        QuickSort(a,left,j-1);
        QuickSort(a,j+1,right);
    }
}


/*  大到小
void QuickSort(int *a,const int left,const int right)
{
    if (left < right) {
        int i = left,j=right+1,pivot = a[left];
        do {
            do i++; while(a[i]>pivot);
            do j--; while(a[j]<pivot);
            if(i<j) swap(a[i],a[j]);
        } while(i<j);
        swap(a[left],a[j]);
        QuickSort(a,left,j-1);
        QuickSort(a,j+1,right);
    }
}
*/

int main(int argc, char *argv[])
{
    int a[] = {54,3,4,2,34,1,4,124,15,54,12,3};
    int len = sizeof(a)/sizeof(int);
    cout << "陣列長度 " << len <<endl<<endl;
    cout << "排序前："<< endl;
    for (int i = 0; i < len; ++i) 
        cout << a[i] <<" ";

    QuickSort(a,0,len-1);

    cout << endl << endl;
    cout << "排序後："<< endl;
    for (int i = 0; i < len; ++i) 
        cout << a[i] <<" ";
    
    return 0;
}
