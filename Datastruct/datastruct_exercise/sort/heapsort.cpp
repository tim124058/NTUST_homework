#include <iostream>
using namespace std;

void Adjust(int *a,const int root,const int n)
{
    int i,e = a[root];
    for (i=2*root;i<=n;i*=2) {
        if(i<n && a[i]<a[i+1]) i++;
        if(e >= a[i]) break;
        a[i/2] = a[i];
    }
    a[i/2] = e;
}

void HeapSort(int *a,const int n)
{
    int tmp;
    for(int i = n/2;i>=1;i--)
        Adjust(a,i,n);
    for (int i = n-1; i >=1; i--) {
        tmp=a[1];a[1]=a[i+1];a[i+1]=tmp;
        Adjust(a,1,i);
    }
}



int main(int argc, char *argv[])
{
    int a[] = {0,26,5,77,1,61,11,59,15,48,19};
    int len = sizeof(a)/sizeof(int);
    cout << "陣列長度 " << len <<endl<<endl;
    cout << "排序前："<< endl;
    for (int i = 1; i < len; ++i) 
        cout << a[i] <<" ";

    HeapSort(a,len-1);

    cout << endl << endl;
    cout << "排序後："<< endl;
    for (int i = 1; i < len; ++i) 
        cout << a[i] <<" ";
    
    return 0;
}
