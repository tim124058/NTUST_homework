#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <regex>
#include <fstream>
using namespace std;

int port = 0;
string IP;

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

//開始socket
void socketStart()
{
    int sockfd;
    struct sockaddr_in  serv_addr;
    string cmd;
    string sbuf;

    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
        throw string("creating socket error");
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP.c_str());
    serv_addr.sin_port = htons(port);

    connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    recvAndCheck(sockfd, sbuf);
    cout << sbuf << endl;
    while(1){
        cout << "> "; getline(cin, cmd);
        if(cmd.length() == 0) continue;
        sendAndCheck(sockfd, cmd);          //send command
        if(cmd == "exit") break;            //close connect
        recvAndCheck(sockfd, sbuf); 
        cout << sbuf << endl;

        //get file
        if (cmd.substr(0,4) == "get " && sbuf != "file not found") {
            int filesize;
            string filename;
            smatch sm;
            regex_search(sbuf, sm, regex("file : (.*?)\n"));
            filename = sm[1];
            regex_search(sbuf, sm, regex("size : (.*?)\n"));
            filesize = stoi(sm[1]);
            sendAndCheck(sockfd,sbuf = "OK");

            char content[4096];
            fstream fd;
            fd.open(filename,ios::out | ios::binary);
            int bufsize = 4000, r = 0;          // bufsize為想要收的大小, r為實際收到的大小

            //開始接收並存檔
            while(filesize > 0){
                if((r=recv(sockfd, content, (filesize > bufsize)?bufsize:filesize, 0)) <= 0) 
                    throw string("error");
                if(r > filesize) {
                    fd.write(content,filesize);
                    break;
                }
                fd.write(content,r);
                filesize -= r;
            }
            fd.close();
            cout << "檔案傳輸成功!" << endl;
        }
    }

    close(sockfd);
    cout << "連線已關閉..." << endl;
}

int main(int argc, char *argv[])
{
    string usage = "Usage : " + string(argv[0]) + " <IP address> <port num>";

    try{
        //檢查參數個數
        if (argc != 3) throw string("argv error");

        //檢查IP
        IP = argv[1];
        char *tmp;
        int c = 0;
        tmp = strtok(argv[1],".");
        for (c = 0; tmp != NULL; c++) {
            if( stoi(tmp) > 255 || stoi(tmp) < 0 ) throw string("ip error");
            tmp = strtok(NULL,".");
        }
        if(c != 4) throw string("ip error");

        //檢查port
        port = stoi(argv[2]);
        if ( port < 0 || port > 65535 ) throw string("port error");
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
