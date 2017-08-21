#include <iostream>
using namespace std;

template <class T> class stack;

template<class T>
class node
{
friend class stack<T>;
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
class stack
{
public:
    stack (){top=NULL;}
    void push(T);
    void pop();
    bool isEmpty();
private:
    node<T> *top;
};

template<class T> 
void stack<T>::push(T data)
{
    node<T> *newnode = new node<T>(data);
    newnode->next = top;
    top = newnode;
}

template<class T>
void stack<T>::pop()
{
    if (isEmpty()) {
        cout<<"沒東西了！！"<<endl;
    }else{
        cout<<top->data<<endl;
        top = top->next;
    }
}

template<class T>
bool stack<T>::isEmpty()
{
    return top==NULL;
}


int main(int argc, char *argv[])
{
    stack<int> *s = new stack<int>();
    s->pop();
    s->push(9);
    s->push(8);
    s->push(7);
    s->push(6);
    s->push(5);
    s->push(4);
    s->pop();
    s->pop();
    s->pop();
    s->pop();
    s->pop();
    s->pop();
    s->pop();
    s->pop();
    s->push(1);
    s->push(2);
    s->pop();
    s->pop();
    s->pop();

    return 0;
}
