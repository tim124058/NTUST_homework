#include <iostream>
using namespace std;

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
    TreeNode(TreeNode *node){this = node;}
    TreeNode(T data,TreeNode<T> *left=NULL,TreeNode<T> *right=NULL)
    {
        this->data = data;
        this->left = left;
        this->right = right;
    }
private:
    T data;
    TreeNode<T> *left;
    TreeNode<T> *right;
};

template<class T>
class Tree
{
public:
    Tree(){root = NULL;}
    void Insert(const T&);
    void Delete(const T& data){Delete(data,root);}
    void DeleteMax();
    void LevelOrder();
    TreeNode<T> *search(const T&);
    int computsize(TreeNode<T> *);
private:
    TreeNode<T> *root;
    void Delete(const T&,TreeNode<T> *&);
};

template<class T>
void Tree<T>::Insert(const T &data)
{
    TreeNode<T> *p = root,*pp = 0;
    while(p) {
        pp = p;
        if (data < p->data) 
            p = p->left;
        else if(data > p->data)
            p = p->right;
        else
            return;
    }
    p = new TreeNode<T>(data);
    if (root) {
        if (data < pp->data)
            pp->left = p;
        else
            pp->right = p;
        
    }else
        root = p; 
}

template<class T>
void Tree<T>::Delete(const T& data,TreeNode<T> *&nodeptr)
{
    if (nodeptr == NULL)
        throw(string("沒有這個資料可以刪除！！"));
    else{
        if (data < nodeptr->data) 
            Delete(data,nodeptr->left);
        else if(data > nodeptr->data)
            Delete(data,nodeptr->right);
        else if(data == nodeptr->data){
            TreeNode<T> *tmp = nodeptr; 
            if (nodeptr->right == NULL) 
                nodeptr = nodeptr->left;
            else if(nodeptr->left == NULL)
                nodeptr = nodeptr->right;
            else{
                tmp = nodeptr->right;
                while(tmp->left)
                    tmp = tmp->left;
                tmp->left = nodeptr->left;
                tmp = nodeptr;
                nodeptr = nodeptr->right;
            }
            delete tmp;
        }
    }
}

template<class T>
void Tree<T>::DeleteMax()
{
    if(root==NULL) throw(string("沒有資料！請先Insert資料"));
    TreeNode<T> *tmp = root,*pp=0;
    while(tmp->right){
        pp=tmp;
        tmp = tmp->right;
    }
    if(tmp == root)
        root = root->left;
    else
        pp->right = tmp->left;
    delete tmp;
}

template<class T>
void Tree<T>::LevelOrder()
{
    if(root==NULL) throw(string("沒有資料！請先Insert資料"));
    Queue<TreeNode<T> *> q,n,tmp;
    q.push(root);
    while(!q.IsEmpty()) {
        TreeNode<T> *currentNode = q.getfront();
        q.pop();
        if(currentNode){
            cout << currentNode->data <<" ";
            n.push(currentNode->left);
            n.push(currentNode->right);
        }
        if (q.IsEmpty()) {
            cout << endl;
            tmp = q;q=n;n=tmp;
        }
    }
}

template<class T>
TreeNode<T>* Tree<T>::search(const T& data)
{
    TreeNode<T> *currentNode = root;
    while(currentNode){
        if (data < currentNode->data)
            currentNode = currentNode->left;
        else if(data > currentNode->data)
            currentNode = currentNode->right;
        else
            return currentNode;
    }
    return NULL;
}

template<class T>
int Tree<T>::computsize(TreeNode<T> *node)
{
    if(node==NULL) throw(string("沒有這筆資料！"));
    int size=0;
    Queue<TreeNode<T> *> q;
    q.push(node);
    while(!q.IsEmpty()) {
        TreeNode<T> *currentNode = q.getfront();
        q.pop();
        size++;
        if(currentNode->left) q.push(currentNode->left);
        if(currentNode->right)q.push(currentNode->right);
    }
    return size;
}

int main(int argc, char *argv[])
{
    Tree<int> bst_pq;
    int op=0,tmp=0;
    
    while(1) {
        try{
            cout << "1.Insert\n2.Delete\n3.DeleteMax\n4.LevelOrder traversal\n5.computsize\n0.exit\n";
            cout << "請輸入動作：";cin>>op; 
            if (op == 0) 
                return 0;
            else if(op == 1){
                cout << "請輸入要插入的值：";cin>>tmp;
                bst_pq.Insert(tmp);
            }else if(op == 2){
                cout << "請輸入要刪除的值：";cin>>tmp;
                bst_pq.Delete(tmp);
            }else if(op ==3){
                bst_pq.DeleteMax();
            }else if(op == 4){
                bst_pq.LevelOrder();
            }else if(op == 5){
                cout << "請輸入要計算的節點值：";cin>>tmp;
                cout << bst_pq.computsize(bst_pq.search(tmp)) << endl;
            }else{}
        }catch(string err){
             cout << err << endl;
        }
        cout << endl << endl;
    }

    return 0;
}

