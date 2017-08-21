#include <iostream>
using namespace std;

class Chain;

class ChainNode
{
    friend class Chain;
private:
    int data;
    ChainNode *next;
public :
    ChainNode(int element=0,ChainNode *next=NULL)
    {
        data = element;
        this->next = next;
    }
};


class Chain
{
private:
    ChainNode *first;
    ChainNode *last;
public:
    Chain()
    {
        first= last = new ChainNode();
    }
    void push(int);
    int pop();
    void print();
    void delete_show(int);
    void insert(int,int);
};


void Chain::push(int data)
{
    ChainNode *tmp = new ChainNode(data);
    last->next = tmp;
    last = last->next;
}

int Chain::pop()
{
    ChainNode *tmp = first;
    while(tmp->next!=last)
        tmp=tmp->next;
    last = tmp;
    tmp=tmp->next;
    int t = tmp->data;
    last->next = NULL;
    delete tmp;
    return t;

}

void Chain::print()
{
    ChainNode *tmp = first->next;
    while(tmp!=last->next)
    {
        cout<<tmp->data<<" ";
        tmp=tmp->next;
    }
    cout<<endl;
}

void Chain::delete_show(int n)
{
    ChainNode *tmp = first;
    for (int i = 0; i < n; ++i) {
        tmp = tmp->next; 
    }
    ChainNode *tmp2 = tmp->next->next;
    int d = tmp->next->data;
    delete(tmp->next);
    tmp->next=tmp2;
    cout<<d<<endl;
}

void Chain::insert(int index,int d)
{
    ChainNode *tmp = first;
    for (int i = 0; i < index; ++i) {
        tmp = tmp->next; 
    }
    ChainNode *n = new ChainNode(d);
    n->next = tmp->next;
    tmp->next=n;
}

int main(int argc, char *argv[])
{
    Chain *chain = new Chain();       
    chain->push(1);
    chain->push(2);
    chain->push(3);
    chain->push(4);
    chain->push(5);
    chain->print();
    cout<<"#"<<endl;
    chain->insert(0,10);
    chain->delete_show(2);
    cout<<"#"<<endl;
    chain->print();

    return 0;
}
