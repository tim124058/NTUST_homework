#include <iostream>
#include <fstream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <map>              //C++ STL，裡面大概是Red-Black tree。 map的key不成重複；multimap的key可以重複
using namespace std;

pthread_mutex_t mutex;      //互斥鎖

typedef map<string, int,less<string> > map_type;        //共用的資料結構    key型態為string,element為int
map_type m;
map_type::iterator iter;

void *runner(void *f)
{
    string tmp = "", content = *(string *)f;            //將傳過來的指標轉成string

    for (int i = 0; i < content.length()+1; ++i) {
        if(content[i]=='\'' && tmp!="" &&((content[i+1]>='a' && content[i+1]<='z') || (content[i+1]>='A' && content[i+1]<='Z')) )   // 單引號的前後為英文單字的
        {
            tmp += content[i];
            continue;
        }

        if(content[i]>='a' && content[i] <='z') 
            tmp += content[i];
        else if(content[i] >='A' && content[i] <= 'Z')
            tmp += tolower(content[i]);
        else if(tmp != "")
        {
            pthread_mutex_lock(&mutex);         //鎖

            /* critical section */
            iter = m.find(tmp);                 //搜尋
            if(iter != m.end())
                iter->second++;                 //資料重複，所以將次數+1
            else
                m.insert(map_type::value_type(tmp,1));          //插入資料

            pthread_mutex_unlock(&mutex);       //解鎖
            tmp = "";
        }
    }

    pthread_exit(0);                            //結束thread
}

int main(int argc, char *argv[])
{
    int file_count = argc - 1;          //要開幾個檔案
    if(file_count > 3) {
        cout << "檔案數量必須小於3個" <<endl;
        exit(EXIT_FAILURE);
    }else if(file_count <=0){
        cout << "請輸入檔案\n程式執行語法為： ./WordAnal [FirstTextFile] [SecondTextFile] [ThirdTextFile]" <<endl;
        exit(EXIT_FAILURE);
    }

    pthread_t threads[3];               //執行緒
    pthread_attr_t attr;
    pthread_attr_init(&attr);           //取得pthread預設屬性
    pthread_mutex_init(&mutex,NULL);    //初始化mutex

    string f[20];
    fstream fileptr;                    //檔案指標
    for(int i=1;i < argc;i++)
    {
        fileptr.open(argv[i],ios::in);
        if(!fileptr.is_open()) {
            cout << "開啟檔案錯誤\n請輸入正確檔名\n";
            exit(EXIT_FAILURE);
        }else{
            f[i-1].assign((istreambuf_iterator<char>(fileptr)), (istreambuf_iterator<char>()));     //讀入整個檔案到f[i-1]裡
            fileptr.close();
        }
    }

    for (int i = 0; i < file_count; ++i)        //開啟執行緒
        pthread_create(&threads[i],&attr,runner,f+i);
    
    pthread_attr_destroy(&attr);                //釋放attr

    for (int i = 0; i < file_count; ++i)        //等待所有執行緒結束
        pthread_join(threads[i],NULL);
    

    //最後排序
    typedef multimap<int, string,greater<int> > multimap_type;
    multimap_type multi;
    multimap_type::iterator multiter;

    for (iter = m.begin(); iter != m.end(); iter++)
        multi.insert(multimap_type::value_type(iter->second,iter->first));
    


    //輸出
    cout << "次數            單字"<<endl;
    for(multiter = multi.begin();multiter != multi.end();multiter++)
    {
        if(multiter->first < 10)
            cout << "   ";
        else if(multiter->first <100)
            cout << "  ";
        else if(multiter->first < 1000)
            cout << " ";
        cout << multiter->first << "            " << multiter->second << endl;
    }
    
    return 0;
}
