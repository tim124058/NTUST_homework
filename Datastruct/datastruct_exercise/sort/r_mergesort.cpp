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
    copy(mergedList+l,mergedList+n+1,initList+l);
}


void rMergeSort(int *a,int *b,int left,int right)
{
    if(left>=right) return;
    int mid = (left + right)/2;
    rMergeSort(a,b,left,mid);
    rMergeSort(a,b,mid+1,right);
    Merge(a,b,left,mid,right);
}

//int ListMerge(int *a,int *link,const int start1,const int start2)
//{
    //int iResult = 0,i1,i2;
    //for (i1 = start1,i2 = start2;i1 && i2 ;) 
       //if (a[i1] <= a[i2]) {
           //link[iResult] = i1;
           //iResult = i1;i1 = link[i1];
       //}else{
           //link[iResult] = i2;
           //iResult = i2;i2 = link[i2];
       //} 

    //if (i1 == 0) link[iResult] = i2;
    //else link[iResult] = i1;
    //return link[0];
//}


//int rMergeSort(int *a,int *link,const int left,const int right)
//{
    //if(left >= right) return left;
    //int mid = (left + right) / 2;
    //return ListMerge(a,link,rMergeSort(a,link,left,mid),rMergeSort(a,link,mid+1,right));
//}




int main(int argc, char *argv[])
{
    int a[] = {26,5,77,1,61,11,59,15,48,19};
    int len = sizeof(a)/sizeof(int);
    int *b = new int[len];
    cout << "陣列長度 " << len <<endl<<endl;
    cout << "排序前："<< endl;
    for (int i = 0; i < len; ++i) 
    {
        b[i]=0; 
        cout << a[i] <<" ";
    }

    rMergeSort(a,b,0,len-1);

    cout << endl << endl;
    cout << "排序後："<< endl;
    for (int i = 0; i < len; ++i) 
        cout << a[b[i]] <<" ";
    
    return 0;
}
