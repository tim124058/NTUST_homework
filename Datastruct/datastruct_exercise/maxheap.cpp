#include <iostream>
using namespace std;

//interface
template<class T>
class MaxPQ
{
public:
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};


//heap
template<class T>
class MaxHeap:public MaxPQ<T>
{
public:
    MaxHeap (int theCapacity = 10);
    bool IsEmpty() const { return heapSize==0;}
    const T& Top() const; 
    void Push(const T&);
    void Pop();
private:
    T *heap;
    int heapSize;
    int capacity;
};

template<class T>
MaxHeap<T>::MaxHeap(int theCapacity)
{
    if (theCapacity < 1) throw "Capacity must be >= 1.";
    capacity = theCapacity;
    heapSize = 0;
    heap = new T[capacity + 1];
}

template<class T>
const T& MaxHeap<T>::Top() const
{
    if(IsEmpty()) return *(new int(-999));
    return heap[1];
}

template<class T>
void MaxHeap<T>::Push(const T& e)
{
    if (heapSize == capacity) {
        capacity *=2;
        T *tmp = heap;
        heap = new T[capacity +1];
        for (int i = 1; i <= heapSize; ++i)
            heap[i] = tmp[i];
        delete [] tmp;
    }
    int currentNode = ++heapSize;
    while(currentNode != 1 && heap[currentNode/2] < e) {
        heap[currentNode] = heap[currentNode/2];
        currentNode/=2;
    }
    heap[currentNode] = e;
}

template<class T>
void MaxHeap<T>::Pop()
{
    if (IsEmpty()) throw "Heap is empty. Cannot delete.";
    heap[1].~T();

    T lastE = heap[heapSize--];

    int currentNode = 1;
    int child = 2;
    while(child <= heapSize) {
        if (child < heapSize && heap[child]<heap[child+1]) child++;
        if (lastE >= heap[child])break;

        heap[currentNode] = heap[child];
        currentNode = child;child*=2;
    }
    heap[currentNode] = lastE;
}

int main(int argc, char *argv[])
{
    MaxHeap<int> *heap = new MaxHeap<int>(5);
    int value=0;
    int op=0;
    while(1)
    {
        cout << "1.看是非為空" << endl;
        cout << "2.看Heap的Top" << endl;
        cout << "3.Push value" << endl;
        cout << "4.清除Top(Pop)" << endl;
        cout << "0.結束" << endl;
        cout << "請選擇動作：";
        cin >> op;
        try{
            switch (op) {
                case 1:
                    cout <<((heap->IsEmpty())?"True":"False") << endl;
                    break;
                case 2:
                    cout << heap->Top() << endl;
                    break;
                case 3:
                    cout << "請輸入要Push的值：";
                    cin >> value;
                    heap->Push(value);
                    break;
                case 4:
                    heap->Pop();
                    break;
                case 0:
                    return 0;
            }
        }
        catch(const char* err)
        {
            cout<<err<<endl;   
        }
        cout<<endl<<endl;
    }

    return 0;
}
