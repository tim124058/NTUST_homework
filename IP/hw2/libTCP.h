#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <string.h>

#ifndef WINDOW_SIZE
#define WINDOW_SIZE 5
#endif

#ifndef PACK_LEN
#define PACK_LEN 14
#endif

struct packet
{
    bool syn, ack, fin;
    int seq_n, ack_n;
    int data;
};

//將int 包裝成 4byte char array
void packint(int num, char *s);

//將4byte char array 還原成 int
void unpackint(char *s, int &num);

//將packet 包裝成 char array
void pack(struct packet p, char *s);

//將char array 還原成 packet
void unpack(char *s, struct packet &p);

//設定封包內容
void setPacket(struct packet &p, bool syn, bool ack, bool fin, int seq_n, int ack_n, int data);

//檢查封包是否符合格式，若不檢查seq_n和ack_n，則設為-1。
bool checkPacket(const struct packet &p, bool syn, bool ack, bool fin, int seq_n, int ack_n);

//set recv timeout = 0.1s
int setRecvTimeout(int fd);

//remove recv timeout
int rmRecvTimeout(int fd);


class SlideWindow
{
private:
	bool received[WINDOW_SIZE];
	int head;		//tail = (head!=0)?head-1:WINDOW_SIZE-1;
	int head_seq;
public:
	SlideWindow(int head_seq);
	int insert(int seq);
};
