#include <iostream>
using namespace std;

template <class T> class queue;

template<class T>
class node
{
friend class queue<T>;
public:
    node (){}
    node(T data,node<T> *next=NULL);
private:
    T data;
    node<T> *next;
};

template<class T>
node<T>::node(T data,node<T> *next)
{
    this->data = data;
    this->next = next;
}


template <class T>
class queue
{
public:
    queue (){front=NULL;}
    void push(T);
    void pop();
    bool isEmpty();
private:
    node<T> *front;
    node<T> *rear;
};

template<class T> 
void queue<T>::push(T data)
{
    if (isEmpty()) {
        front = rear = new node<T>(data);
    }else
    {
        rear->next = new node<T>(data);
        rear = rear->next;
    }
}

template<class T>
void queue<T>::pop()
{
    if (isEmpty()) {
        cout<<"沒東西了！！"<<endl;
    }else{
        node<T>  *temp = front;
        cout<<front->data<<endl;
        front = front->next;
        delete temp;
    }
}

template<class T>
bool queue<T>::isEmpty()
{
    return front==NULL;
}


int main(int argc, char *argv[])
{
    queue<char> *q = new queue<char>();
    q->pop();
    q->push('A');
    q->push('B');
    q->push('C');
    q->push('D');
    q->push('E');
    q->pop();
    q->pop();
    q->pop();
    q->pop();
    q->pop();
    q->pop();
    q->push('F');
    q->push('G');
    q->pop();
    q->pop();
    q->pop();
    return 0;
}
