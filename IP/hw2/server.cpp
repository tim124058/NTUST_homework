#include "libTCP.h"
using namespace std;

int port = 0;
unsigned int packetN = 0;		//number of packets
struct packet rec_p, send_p; 

//只收跟cli_addr一樣ip和port的封包
int recvfromUDP(int sockfd, struct packet &p, struct sockaddr_in cli_addr)
{
	socklen_t addr_len = sizeof(cli_addr);
	struct sockaddr_in rec_addr;
	char buf[PACK_LEN];
	int n;
	while(1){
		socklen_t rec_len = addr_len;
		n =  recvfrom(sockfd, buf, PACK_LEN , 0, (struct sockaddr *)&rec_addr, &rec_len);
		if(n < 0) return -1;
		if(rec_len != addr_len || memcmp(&rec_addr, &cli_addr, addr_len) != 0)
			continue;

		if (n != PACK_LEN) return -1;
		unpack(buf, p);
		cout << "Receive : seq = " << p.seq_n << ", ack = " << p.ack_n << endl;
		return n;
	}
}

//將send_p送到cli_addr
int sendtoUDP(int sockfd, struct packet p, struct sockaddr_in cli_addr)
{
	char buf[PACK_LEN];
	int n;
	pack(p, buf);
   	n = sendto(sockfd, buf, PACK_LEN, 0,(struct sockaddr *)&cli_addr, sizeof(cli_addr));
	if (n != PACK_LEN) return -1;
	cout << "Send : seq = " << p.seq_n << ", ack = " << p.ack_n << endl;
	return n;
}

//等待並開始three-way handshake，成功連線回傳1，失敗回傳0，並存入cli_addr。
int threeWayHandshake(int sockfd, struct sockaddr_in &cli_addr){
	char buf[PACK_LEN];
	int n = 0;	//recv or send len
    socklen_t addr_len = sizeof(cli_addr);

    cout << "\n\nwait connect..." << endl;
    if(rmRecvTimeout(sockfd) < 0) return 0;

    //blocking and recv SYN
    n =  recvfrom(sockfd, buf, PACK_LEN , 0, (struct sockaddr *)&cli_addr, &addr_len);
    cout << "[Start three-way handshake]" << endl;
    if (n != PACK_LEN) return 0;
    unpack(buf, rec_p);
    if(!checkPacket(rec_p,1,0,0,-1,-1)) return 0;
    cout << "Receive : seq = " << rec_p.seq_n << ", ack = " << rec_p.ack_n << endl;

    if(setRecvTimeout(sockfd) < 0) return 0;

   	//send SYN,ACK
   	setPacket(send_p,1,1,0,10,rec_p.seq_n + 1,0);
   	if(sendtoUDP(sockfd, send_p, cli_addr) <=0) return 0;

   	//recv ACK
	if(recvfromUDP(sockfd, rec_p, cli_addr) <=0) return 0;
	if(!checkPacket(rec_p,0,1,0,send_p.ack_n,(send_p.seq_n+1))) return 0;

	return 1;
}

//送出seqence在head_seq~tail_seq之間的所有封包
void sendWindow(int sockfd, int head_seq, int tail_seq, const struct sockaddr_in &cli_addr)
{
    for (int i = head_seq; i <= tail_seq; ++i){
        //send ACK
        setPacket(send_p,0,1,0,i,rec_p.seq_n,123);
        if(sendtoUDP(sockfd, send_p, cli_addr) <=0) cout << "send error\n";
    }
}

//transmission
int sendNPacket(int sockfd, const struct sockaddr_in &cli_addr)
{
    int timeoutFlag = 0 ,duplicate = 0;
    int head_seq = rec_p.ack_n + 1;
    int tail = head_seq + packetN - 1;
    int tail_seq = ((head_seq + WINDOW_SIZE - 1) > tail)?tail:(head_seq + WINDOW_SIZE - 1);

    sendWindow(sockfd,head_seq,tail_seq,cli_addr);
    while(head_seq <= tail){
        if(recvfromUDP(sockfd, rec_p, cli_addr) == -1){             //timeout
            cout << "# Timeout and retransmission" << endl;
            sendWindow(sockfd,head_seq,tail_seq,cli_addr);
            if (timeoutFlag > 3){
                cout << "# 連續timeout太多次，client可能已經關閉連線" << endl;
                return 0;
            }
            timeoutFlag++;
            continue;
        }
        timeoutFlag = 0;
        if(!checkPacket(rec_p,0,1,0,-1,-1)){       //丟棄不是ack的封包
            cout << "...discard\n";continue;
        }
        if(rec_p.ack_n >= head_seq && rec_p.ack_n <= tail_seq){     //正確的收到ack
            duplicate = 0;
            int tmptail = tail_seq;
            head_seq = rec_p.ack_n+1;              
            tail_seq = ((head_seq + WINDOW_SIZE - 1) > tail)?tail:(head_seq + WINDOW_SIZE - 1);
            sendWindow(sockfd,tmptail+1,tail_seq,cli_addr);         //送出window中，還沒送過的封包
        }else if(rec_p.ack_n == head_seq-1){        //收到重複的ack
            duplicate++;
            if(duplicate >= 2){
                duplicate = 0;
                cout << "# Three duplicate acks and retransmission" << endl;
                sendWindow(sockfd,head_seq,tail_seq,cli_addr);
            }
        }
    }
    //清空receive buffer
    while(recvfromUDP(sockfd, rec_p, cli_addr) != -1) cout << "...discard\n";
    return 1;
}

//中斷連線
int terminate(int sockfd, struct sockaddr_in &cli_addr)
{
    //send FIN
    setPacket(send_p,0,0,1,rec_p.ack_n+1,rec_p.seq_n+1,0);
    if(sendtoUDP(sockfd, send_p, cli_addr) <=0) return 0;

    //recv ACK
    if(recvfromUDP(sockfd, rec_p, cli_addr) <=0) return 0;
    if(!checkPacket(rec_p,0,1,0,-1,(send_p.seq_n+1))) return 0;

    //recv ACK,FIN
    if(recvfromUDP(sockfd, rec_p, cli_addr) <=0) return 0;
    if(!checkPacket(rec_p,0,1,1,-1,(send_p.seq_n+1))) return 0;

    //send ACK
    setPacket(send_p,0,1,0,rec_p.ack_n,rec_p.seq_n + 1,0);
    if(sendtoUDP(sockfd, send_p, cli_addr) <=0) return 0;

    memset(&cli_addr, 0, sizeof(cli_addr));
    return 1;
}

//開始socket
void socketStart()
{
    int sockfd;
    struct sockaddr_in  serv_addr, cli_addr;

    if ((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
        throw string("creating socket error");
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    ::bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    while(1){
        try{
    	    if(threeWayHandshake(sockfd, cli_addr) <= 0){
    	    	cerr << "[three-way handshake error]" << endl;
    	    	continue;
    	    }
    	    cout << "[three-way handshake successed]\n" << endl;
    	  	
            cout << "[Start transmission]" << endl;
            if(sendNPacket(sockfd,cli_addr))
                cout << "[Transmission successed]\n" << endl;
            else
                cerr << "[Transmission error]\n" << endl;
            
            cout << "[Start Terminate]" << endl;
            if (terminate(sockfd, cli_addr))
                cout << "[Terminate successed]\n" << endl;
            else 
                cerr << "[Terminate error]\n" << endl;
            cout << endl;
        }catch(...){
            cerr << "[unknow error...]"<< endl;
        }
    }

    close(sockfd);
}

int main(int argc, char *argv[])
{
    int i;
    string usage = "Usage : " + string(argv[0]) + " <port> <num of packets>";

    try{
        //檢查參數個數
        if (argc != 3) throw string("argv error");

        //檢查port
        port = stoi(argv[1]);
        if ( port < 0 || port > 65535 ) throw string("port error");

        packetN = stoi(argv[2]);
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
