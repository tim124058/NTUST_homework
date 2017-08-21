#include<iostream>
#include<queue>
using namespace std;

template<class T> class Tree;

template<class T>
class TreeNode
{
//friend class Tree<T>;
public:
    TreeNode(){}
    TreeNode(TreeNode *node){this = node;}
    TreeNode(T data,TreeNode<T> *leftChild=NULL,TreeNode<T> *rightChild=NULL);
//protected:            //friend不能繼承(BST不能用)
    T data;
    TreeNode<T> *leftChild;
    TreeNode<T> *rightChild;
};

template<class T>
TreeNode<T>::TreeNode(T data,TreeNode<T> *leftChild,TreeNode<T> *rightChild)
{
    this->data = data;
    this->leftChild = leftChild;
    this->rightChild = rightChild;
}


template<class T>
class Tree
{
public:
    Tree(){root = NULL;}                                //創一個空的樹
    Tree(TreeNode<T>* node){this->root=node;}           //傳入整棵樹
    Tree(TreeNode<T>* bt1,T data,TreeNode<T> *bt2);     //傳入2棵樹+1data組成新的樹
    Tree<T> LeftSubTree();
    Tree<T> RightSubTree();
    T RootData(){return root->data;}
    bool IsEmpty(){return root==NULL;}
    void Inorder(){Inorder(root);}
    void Preorder(){Preorder(root);}
    void Postorder(){Postorder(root);}
    void Levelorder();
    virtual void visit(TreeNode<T> *node) = 0;//{cout << node->data << endl ;}
protected:
    TreeNode<T> *root; 
    void Inorder(TreeNode<T> *);
    void Preorder(TreeNode<T> *);
    void Postorder(TreeNode<T> *);
};

template<class T>
Tree<T>::Tree(TreeNode<T>* left,T data,TreeNode<T> *right)
{
    root->data = data;
    root->leftChild = left;
    root->rightChild = right;
}

template<class T>
Tree<T> Tree<T>::LeftSubTree()
{
    return new Tree(root->LeftSubTree);
}

template<class T>
Tree<T> Tree<T>::RightSubTree()
{
    return new Tree(root->RightSubTree);
}

template<class T>
void Tree<T>::Inorder(TreeNode<T> *currentNode)
{
    if (currentNode) {
        Inorder(currentNode->leftChild);
        visit(currentNode);
        Inorder(currentNode->rightChild);
    }   
}

template<class T>
void Tree<T>::Preorder(TreeNode<T> *currentNode)
{
    if (currentNode) {
        visit(currentNode);
        Preorder(currentNode->leftChild);
        Preorder(currentNode->rightChild);
    }   
}

template<class T>
void Tree<T>::Postorder(TreeNode<T> *currentNode)
{
    if (currentNode) {
        Postorder(currentNode->leftChild);
        Postorder(currentNode->rightChild);
        visit(currentNode);
    }   
}

template<class T>
void Tree<T>::Levelorder()
{
    queue<TreeNode<T> *> q;
    TreeNode<T> *currentNode = root;
    while(currentNode)
    {
        visit(currentNode);
        if(currentNode->leftChild) q.push(currentNode->leftChild);
        if(currentNode->rightChild) q.push(currentNode->rightChild);
        if(q.empty()) return;
        currentNode = q.front();
        q.pop();
    }
}


//binary search tree
template<class K,class E>
class BST:public Tree<pair<K,E> >
{
public:
    BST():Tree<pair<K,E> >(){}                             
    BST(TreeNode<pair<K,E> >* node):Tree<pair<K,E> >(node){}       
    BST(TreeNode<pair<K,E> >* bt1,pair<K,E> data,TreeNode<pair<K,E> > *bt2):Tree<pair<K,E> >(bt1,data,bt2){}
    pair<K,E>* Get(const K& k){return Get(this->root,k);}       //遞迴版
    pair<K,E>* Get_while(const K&);                             //while版本的
    void Insert(const pair<K,E>&);
    void Delete(const K& key){Delete(key,this->root);}
    void visit(TreeNode<pair<K,E> > *node){cout << "key : " << node->data.first << "  "<< node->data.second << endl;}
private:
    pair<K,E>* Get(TreeNode<pair<K,E> >*, const K&);
    void Delete(const K&,TreeNode<pair<K,E> > *&);
};

template<class K,class E>
pair<K,E>* BST<K,E>::Get(TreeNode<pair<K,E> >* p, const K& k)
{
    if (!p) return 0;
    if(k < p->data.first) return Get(p->leftChild,k);
    if(k > p->data.first) return Get(p->rightChild,k);
    return &p->data;
}

template<class K,class E>
pair<K,E>* BST<K,E>::Get_while(const K& k)
{
    TreeNode<pair<K,E> >* currentNode = this->root;
    while(currentNode) {
        if (k < currentNode->data.first)
            currentNode = currentNode->leftChild;
        else if(k > currentNode->data.first)
            currentNode = currentNode->right;
        else
            return currentNode->data;
    }
    return 0;
}

template<class K,class E>
void BST<K,E>::Insert(const pair<K,E>& thePair)
{
    TreeNode<pair<K,E> > *p = this->root,*pp = 0;
    while(p) {
        pp = p;
        if(thePair.first < p->data.first) 
            p = p->leftChild;
        else if(thePair.first > p->data.first) 
            p = p->rightChild;
        else
        {
            p->data.second = thePair.second;
            return ;   
        }
    }
    
    p = new TreeNode<pair<K,E> >(thePair);
    if (this->root) {
        if (thePair.first < pp->data.first) 
            pp->leftChild = p;
        else
            pp->rightChild = p;
    }else
        this->root = p;
}

template<class K,class E>
void BST<K,E>::Delete(const K& key,TreeNode<pair<K,E> > *&nodeptr)
{
    if (nodeptr == NULL)
        throw(string("\n不能刪除空節點！"));
    else 
    {
        if (key < nodeptr->data.first) 
            Delete(key,nodeptr->leftChild);
        else if(key > nodeptr->data.first)
            Delete(key,nodeptr->rightChild);
        else if(key == nodeptr->data.first)         //找到了
        {
            TreeNode<pair<K,E> > *tmp = nodeptr;
            if (nodeptr->rightChild == NULL)             //右邊空的或兩邊都空的
                nodeptr = nodeptr->leftChild;
            else if(nodeptr->leftChild == NULL)
                nodeptr = nodeptr->rightChild;
            else
            {
                tmp = nodeptr->rightChild;
                while(tmp->leftChild)
                    tmp = tmp->leftChild;
                tmp->leftChild = nodeptr->leftChild;
                tmp = nodeptr;
                nodeptr = nodeptr->rightChild;
            }
            delete tmp;
        }
    }
}

int main(int argc, char *argv[])
{
    BST<int,string> *tr = new BST<int,string>();
    pair<int,string> inp;
    pair<int,string> *tmp;
    int op = 0; 
    int key=0;
    string element = "";

    while(1)
    {   
        cout << "1.插入節點\n";
        cout << "2.刪除節點\n";
        cout << "3.Inorder輸出\n";
        cout << "4.Preorder輸出\n";
        cout << "5.Postoder輸出\n";
        cout << "6.Leveloder輸出\n";
        cout << "7.看是否為空的\n";
        cout << "8.visit自輸入節點資料\n";
        cout << "9.回傳root節點\n";
        cout << "10.用get取得資料\n";
        cout << "11.用get_while取得資料\n";
        cout << "0.結束程式\n";
        cout << "輸入動作：";   cin >> op;
        
        try{
            if (op==0) 
                return 0;
            else if (op==1) {
                cout << "輸入 key : " ; cin >> key;
                cout << "輸入資料 : "; cin >> element;cout <<endl;
                inp.first = key;
                inp.second = element;
                tr->Insert(inp);
                cout << "#輸入成功....";

            } 
            else if (op==2) {
                if (tr->IsEmpty()) throw(string("\n空的！請先輸入資料！"));
                cout << "輸入要刪除的key : "; cin >> key; cout <<endl;
                tr->Delete(key);
                cout << "#刪除成功....";
            } 
            else if (op==3){
                if (tr->IsEmpty()) throw(string("\n空的！請先輸入資料！"));
                cout <<endl;
                tr->Inorder();
            }
            else if (op==4){
                if (tr->IsEmpty()) throw(string("\n空的！請先輸入資料！"));
                cout <<endl;
                tr->Preorder();
            }
            else if (op==5){
                if (tr->IsEmpty()) throw(string("\n空的！請先輸入資料！"));
                cout <<endl;
                tr->Postorder();
            }
            else if (op==6){
                if (tr->IsEmpty()) throw(string("\n空的！請先輸入資料！"));
                cout <<endl;
                tr->Levelorder();
            }
            else if (op==7) {
                cout << endl <<(tr->IsEmpty()?"True":"False") << endl;
            } 
            else if (op==8) {
                cout << "輸入 key : " ; cin >> key;
                cout << "輸入資料 : "; cin >> element;cout <<endl;
                inp.first = key;
                inp.second = element;
                tr->visit(new TreeNode<pair<int,string> >(inp));
            } 
            else if (op==9) {
                if (tr->IsEmpty()) throw(string("\n空的！請先輸入資料！"));
                cout <<endl;
                tr->visit(new TreeNode<pair<int,string> >(tr->RootData()));
            } 
            else if (op==10) {
                if (tr->IsEmpty()) throw(string("\n空的！請先輸入資料！"));
                cout << "輸入要get的key : "; cin >> key; cout <<endl;
                tmp = tr->Get(key);
                tr->visit(new TreeNode<pair<int,string> >(*tmp));
            } 
            else if (op==11) {
                if (tr->IsEmpty()) throw(string("\n空的！請先輸入資料！"));
                cout << "輸入要get的key : "; cin >> key; cout <<endl;
                tmp = tr->Get(key);
                tr->visit(new TreeNode<pair<int,string> >(*tmp));
            } 
            else {} 
        }catch(string err)
        {
             cout << err << endl;
        }
        cout << endl << endl << endl;
    }

    return 0;
}
