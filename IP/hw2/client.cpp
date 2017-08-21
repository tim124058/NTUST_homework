#include "libTCP.h"
using namespace std;
int DEBUG = 0;        //0=>不做debug    1=>Three duplicate    2=>timeout

int port = 0;
string IP;
struct packet rec_p, send_p; 

//只收跟cli_addr一樣ip和port的封包
int recvUDP(int sockfd, struct packet &p)
{
    char buf[PACK_LEN];
    int n;
    n =  recv(sockfd, buf, PACK_LEN , 0);
    if (n != PACK_LEN) return -1;
    unpack(buf, p);
    cout << "Receive : seq = " << p.seq_n << ", ack = " << p.ack_n << endl;
    return n;
}

//將send_p送到cli_addr
int sendUDP(int sockfd, struct packet p)
{
    char buf[PACK_LEN];
    int n;
    pack(p, buf);
    n = send(sockfd, buf, PACK_LEN, 0);
    if (n != PACK_LEN) return -1;
    cout << "Send : seq = " << p.seq_n << ", ack = " << p.ack_n << endl;
    return n;
}

//發起連線
int threeWayHandshake(int sockfd, struct sockaddr_in &serv_addr)
{
    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) return 0;
    
    if(setRecvTimeout(sockfd) < 0) return 0;

    //send SYN
    setPacket(send_p,1,0,0,20,0,0);
    if(sendUDP(sockfd, send_p) <=0) return 0;

    //recv SYN,ACK
    if(recvUDP(sockfd, rec_p) <=0) return 0;
    if(!checkPacket(rec_p,1,1,0,-1,(send_p.seq_n+1))) return 0;

    //send ACK
    setPacket(send_p,0,1,0,rec_p.ack_n,rec_p.seq_n + 1,0);
    if(sendUDP(sockfd, send_p) <=0) return 0;

    if(rmRecvTimeout(sockfd) < 0) return 0;
    return 1;
}

//一直收到Fin封包出現
int recvUntilFin(int sockfd)
{
    SlideWindow window(send_p.ack_n+1);
    int tmp = -1;
    int hs1tmp = -1,head_seq_1,duplicate = WINDOW_SIZE;

    while (true){
        recvUDP(sockfd, rec_p);
        if(checkPacket(rec_p,0,0,1,-1,-1)) {        //收到FIN封包，結束recv
            cout << "Received FIN packet" << endl;
            return 1;
        }
        if (!checkPacket(rec_p,0,1,0,-1,-1)){       //丟棄不是ack的封包
            cout << "...discard\n";continue;
        }
        if (DEBUG==1 && tmp == -1 && rec_p.seq_n == 20){
            cout << "...discard(DEBUG)\n";tmp = 20;continue;
        }else if (DEBUG==2 && tmp < 3 && rec_p.seq_n >= 20){
            tmp++;cout << "...discard(DEBUG)\n";continue;
        }

        if( (head_seq_1 = window.insert(rec_p.seq_n)) == -1){   //丟棄超出範圍，或已收過的封包
            cout << "...discard\n";continue;
        }
        if(hs1tmp == head_seq_1){                   //防止WINDOW_SIZE太大，一直造成three duplicate
            duplicate--;
            if(duplicate <= 0) duplicate = WINDOW_SIZE;
            if(duplicate <= WINDOW_SIZE - 4) continue;
        }else duplicate = WINDOW_SIZE;
        hs1tmp = head_seq_1;
        cout << "Data = " << rec_p.data << endl;
        //send ACK
        setPacket(send_p,0,1,0,rec_p.ack_n,head_seq_1,0);
        if(sendUDP(sockfd, send_p) <=0) cout << "send error" <<endl;
    }
}

//中斷連線
int terminate(int sockfd)
{ 
    if(setRecvTimeout(sockfd) < 0) return 0;

    //send ACK
    setPacket(send_p,0,1,0,rec_p.ack_n,rec_p.seq_n + 1,0);
    if(sendUDP(sockfd, send_p) <=0) return 0;

    //send ACK,FIN
    setPacket(send_p,0,1,1,rec_p.ack_n,rec_p.seq_n + 1,0);
    if(sendUDP(sockfd, send_p) <=0) return 0;

    //recv ACK
    if(recvUDP(sockfd, rec_p) <=0) return 0;
    if(!checkPacket(rec_p,0,1,0,send_p.ack_n,(send_p.seq_n+1))) return 0;

    return 1;
}

//開始socket
void socketStart()
{
    int sockfd;
    struct sockaddr_in  serv_addr;

    if ((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
        throw string("creating socket error");
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP.c_str());
    serv_addr.sin_port = htons(port);

    cout << "[Start three-way handshake]" << endl;
    if(threeWayHandshake(sockfd, serv_addr) <= 0) {
        close(sockfd);
        cerr << "[three-way handshake error]" << endl;
        return;
    }
    cout << "[three-way handshake successed]\n" << endl;
    
    cout << "Start receive" << endl;
    if(recvUntilFin(sockfd))
        cout << "[Receive successed]\n" << endl;
    else
        cerr << "[Receive error]\n" << endl;
    
    cout << "[Start terminate]" << endl;
    if (terminate(sockfd))
        cout << "[Terminate successed]\n" << endl;
    else 
        cerr << "[Terminate error]\n" << endl;

    close(sockfd);
    cout << "連線已關閉..." << endl;
}

int main(int argc, char *argv[])
{
    string usage = "Usage : " + string(argv[0]) + " <IP> <port>";

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
        cerr << message << endl;
        cerr << usage << endl;
        exit(1);
    }catch(...){
        cerr << usage << endl;
        exit(1);
    }

    socketStart();
    return 0;
}
