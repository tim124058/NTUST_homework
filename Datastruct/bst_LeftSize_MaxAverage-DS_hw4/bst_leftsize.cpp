#include <iostream>
#include <sstream>
using namespace std;

stringstream s;
string tmp="";

//實作queue
template<class T> class Queue;
template<class T>
class node
{
friend class Queue<T>;
public:
    node(){}
    node(T data,node<T> *next=NULL) {
        this->data = data;
        this->next = next;
    }
private:
    T data;
    node<T> *next;
};

template<class T>
class Queue
{
public:
    Queue (){front = rare = NULL;};
    bool IsEmpty(){return front == NULL;}
    T getfront(){return front->data;}
    void push(T data){
        if (IsEmpty()) 
            front = rare = new node<T>(data);
        else {
            rare->next = new node<T>(data);
            rare = rare->next;
        }
    }
    void pop(){
        if (!IsEmpty()){
            node<T> *tmp = front;
            front = front->next;
            delete tmp;
        }
    }
private:
    node<T> *front;
    node<T> *rare;
};

//以binary search tree完成max priority queue
template<class T> class Tree;
template<class T>
class TreeNode
{
friend class Tree<T>;
public:
    TreeNode(){this=NULL;}
    TreeNode(T data,TreeNode<T> *left=NULL,TreeNode<T> *right=NULL)
    {
        this->left_count = 1;
        this->data = data;
        this->left = left;
        this->right = right;
    }
private:
    T data;
    int left_count;
    TreeNode<T> *left;
    TreeNode<T> *right;
};

template<class T>
class Tree
{
public:
    Tree(){root = NULL;nodecount=0;}
    void Insert(const T&);
    void LevelOrder();
    string AverageN(int n);
    bool IsEmpty(){return !root;}
private:
    TreeNode<T> *root;
    int nodecount;
    void AverageN(TreeNode<T> *,string &,int &,double &);
};

template<class T>
void Tree<T>::Insert(const T &data)
{
    TreeNode<T> *p = root,*pp = 0;
    while(p) {
        pp = p;
        if (data < p->data) {
            p->left_count++;
            p = p->left;
        }
        else if(data > p->data)
            p = p->right;
        else {
            p=root;
            while(p) {
                if (data < p->data) {
                    p->left_count--;
                    p = p->left;
                }
                else if(data > p->data)
                    p = p->right;
                else
                    return;
            }
            return;
        }
    }
    p = new TreeNode<T>(data);
    nodecount++;
    if (root) {
        if (data < pp->data)
            pp->left = p;
        else
            pp->right = p;
        
    }else
        root = p; 
}

template<class T>
void Tree<T>::LevelOrder()
{
    if(root==NULL) throw(string("沒有資料！請先Insert資料"));
    Queue<TreeNode<T> *> q;
    q.push(root);
    while(!q.IsEmpty()) {
        TreeNode<T> *currentNode = q.getfront();
        q.pop();
        if(currentNode){
            cout << currentNode->left_count <<" ";
            q.push(currentNode->left);
            q.push(currentNode->right);
        }
    }
}

template<class T>
string Tree<T>::AverageN(int n)
{
    if(n<0 || n>nodecount) return "-1";
    string a="(";
    int i=n;
    double sum=0;
    AverageN(root,a,i,sum);
    a = a.substr(0,a.size()-1);
    s<<n;s>>tmp;
    a=a+")/"+ tmp +" = ";
    s.str(""); s.clear();
    s<<sum/n;s>>tmp;
    a+=tmp;
    s.str(""); s.clear();
    return a;
}

template<class T>
void Tree<T>::AverageN(TreeNode<T> *currentNode,string &a,int &n,double &sum)
{
    if (currentNode && n>0) {
        AverageN(currentNode->right,a,n,sum);
        if(n>0){
            sum+=currentNode->data;
            s<<currentNode->data;s>>tmp;
            s.str(""); s.clear();
            a += tmp+"+";
            n--;
            AverageN(currentNode->left,a,n,sum);
        }
    }
}


int main(int argc, char *argv[])
{
    Tree<int> bst_pq;
    string elements="";
    int n=0,inttmp;
    cout << "Input:";
    getline(cin,elements);
    elements+=" ";
    for (int i = 0; i < elements.size(); ++i) {
        if(elements[i]<'0' || elements[i]>'9') {
            if(tmp!="") {
                s << tmp;
                s >>inttmp;
                bst_pq.Insert(inttmp);
                s.str(""); s.clear();
            }
            tmp="";
        }
        else
            tmp+=elements[i];
    }
    if(bst_pq.IsEmpty()) return 0;
    cout << "\nLevel-order output : ";
    bst_pq.LevelOrder();
    cout << endl;

    while(1) {
        cout << "請輸入要幾個最大平均數(0為結束程式)：" ;cin >> n;
        if (n==0) 
            return 0;
        cout <<"Average of top " << n <<" elements:" << bst_pq.AverageN(n) << endl << endl;
    }

    return 0;
}

