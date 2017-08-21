#include <iostream>
using namespace std;

void Merge(int *initList, int *mergedList, const int l, const int m, const int n)
{ 
    int i1,iResult,i2;
    for (i1=l,iResult=l,i2=m+1; i1<=m && i2<=n;iResult++) {
        if (initList[i1] <= initList[i2]) {
            mergedList[iResult] = initList[i1];i1++;
        }else{
            mergedList[iResult] = initList[i2];i2++;
        }
    }
    if(i1<=m)
        copy(initList+i1, initList+m+1, mergedList+iResult);
    else
        copy(initList+i2, initList+n+1, mergedList+iResult);
}

void MergePass(int *initList,int *resultList, const int n, const int s)
{
    int i;
    for(i=0;i<= n-2*s ;i+=2*s)
        Merge(initList, resultList,i,i+s-1,i+2*s-1);
    if(i+s <n) Merge(initList,resultList,i,i+s-1,n-1);
    else if(i<n) copy(initList+i,initList+n+1,resultList+i);
}

void MergeSort(int *a,const int n)
{
    int *tempList = new int[n];
    for (int i = 1; i < n; i*=2) {
        MergePass(a,tempList,n,i);
        i*=2;
        MergePass(tempList,a,n,i);
    }
    delete [] tempList;
}

int main(int argc, char *argv[])
{
    //int a[] = {54,3,4,2,34,1,4,124,15,54,12,3};
    //int len = sizeof(a)/sizeof(int);
    int a[100],len=0,i=0;
    while(1)
    {
        cin >>i;
        if(i==-1)break;
        a[len]=i;
        len++;
    }
    cout << "陣列長度 " << len <<endl<<endl;
    cout << "排序前："<< endl;
    for (int i = 0; i < len; ++i) 
        cout << a[i] <<" ";

    MergeSort(a,len);

    cout << endl << endl;
    cout << "排序後："<< endl;
    for (int i = 0; i < len; ++i) 
        cout << a[i] <<" ";
    
    return 0;
}
