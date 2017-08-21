#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fstream>
using namespace std;

int port = 0;
int filenum = 0;            //有幾個可下載的檔案
string *files;              //所有可下載的檔名

void recvAndCheck(int &sockfd,string &sbuf)
{
    char buf[4096];
    bzero(&buf, sizeof(buf));
    if(recv(sockfd, buf, sizeof(buf) - 1, 0) <= 0) 
        throw string("error");
    sbuf = buf;
}

void sendAndCheck(int &sockfd,string &sbuf)
{
    if(send(sockfd, sbuf.c_str(), sbuf.length(), 0) <= 0) 
        throw string("error");
}

//檢查client輸入的是否為可下載的檔案
bool fileExist(string filename)
{
    for (int i = 0; i < filenum; ++i) {
        if(filename == files[i] || filename == ("%"+(to_string(i))))
            return true;
    }
    return false;
}

//開始socket
void socketStart()
{
    string Help = "請輸入指令 : \n\
  help 顯示幫助訊息\n\
  ls 列出有哪些檔案可以下載\n\
  get filename 下載檔案(get %num)\n\
  exit 結束\n";

    int sockfd;
    struct sockaddr_in  serv_addr, cli_addr;
    string sbuf;
    int client_port;
    char client_ip[30];

    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
        throw string("creating socket error");
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    ::bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));        // ::bind 不使用std::bind(c++11)
    listen(sockfd, 5);
    cout << "等待連線...\n" << endl;
                    
    while(1){
        try{
            int clientfd;
            socklen_t len;
            struct sockaddr_in cli_addr;
            len = sizeof(cli_addr);
            clientfd = accept(sockfd, (struct sockaddr*)&cli_addr, &len);           //等待並接受連線
            inet_ntop(AF_INET, &cli_addr.sin_addr, client_ip, sizeof(client_ip));
            client_port = ntohs(cli_addr.sin_port);
            cout << "Client " << client_ip << ":" << client_port << " 已連線" << endl;

            sbuf = "連線成功...\n\n" + Help ;
            sendAndCheck(clientfd, sbuf);

            //command line
            while(1){
                recvAndCheck(clientfd, sbuf);
                if(sbuf != "") 
                    cout << client_ip << ":" << client_port << " ; cmd = '" << sbuf << "'" << endl;

                if(sbuf.substr(0,4) == "get "){         //send file
                    string filename = sbuf.substr(4);
                    if(fileExist(filename)){
                        if(filename[0] == '%')
                            filename = files[stoi(filename.substr(1))];
                        fstream fd;

                        //計算檔案大小
                        int filesize;
                        fd.open(filename,ios::in | ios::binary | ios::ate);
                        filesize = fd.tellg();
                        fd.seekg(0, ios::beg);

                        sbuf = "file : " + filename + "\nsize : " + to_string(filesize) + "\n";
                        sendAndCheck(clientfd,sbuf);
                        recvAndCheck(clientfd,sbuf);
                        
                        //讀檔並傳送檔案
                        char content[4096];
                        int bufsize = 4000;
                        while(filesize >= bufsize){
                            fd.read(content,bufsize);
                            if(send(clientfd, content, bufsize, 0) <= 0) throw string("error");
                            filesize -= bufsize;
                        }
                        if(filesize > 0){
                            fd.read(content,filesize);
                            if(send(clientfd, content, filesize, 0) <= 0) throw string("error");
                        }
                        fd.close();

                    }else
                        sendAndCheck(clientfd,sbuf = "file not found");
                }else if (sbuf == "help") {             //help message
                    sendAndCheck(clientfd, Help);
                }else if (sbuf == "ls") {               //list file
                    sbuf = "";
                    for (int i = 0; i < filenum; ++i)
                        sbuf += "%" + to_string(i) + " : " + files[i] + "\n";
                    sendAndCheck(clientfd, sbuf);
                }else if(sbuf == "exit"){               //close connect
                    break;
                }else{
                    sendAndCheck(clientfd, sbuf = "command not found");
                }
            }
            close(clientfd);
            cout << "Client " << client_ip << ":" << client_port << " 連線已關閉" << endl;
            
        }catch(...){                //防止client使用ctrl+c關閉程式而crash
            cout << "Client " << client_ip << ":" << client_port << " 連線非預期結束" << endl;
        }
    }

    close(sockfd);
}

//檢查檔案是否存在於server
bool fileExistServ(const string fileName)
{
    ifstream infile(fileName);
    return infile.good();
}

int main(int argc, char *argv[])
{
    int i;
    string usage = "Usage : " + string(argv[0]) + " <port num> <file1> <file2> ...";

    try{
        //檢查參數個數
        if (argc < 3) throw string("argv error");

        //檢查port
        port = stoi(argv[1]);
        if ( port < 0 || port > 65535 ) throw string("port error");

        //檢查檔案
        filenum = argc - 2;
        files = new string[filenum];
        for (i = 0; i < filenum; ++i) {
            files[i] = argv[i+2];
            if(!fileExistServ(files[i])) throw string( files[i] + " : file not found");
        }
    }catch(string message){
        cout << message << endl;
        cout << usage << endl;
        exit(1);
    }catch(...){
        cout << usage << endl;
        exit(1);
    }

    socketStart();
    return 0;
}
