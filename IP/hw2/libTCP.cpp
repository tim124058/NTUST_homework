#include "libTCP.h"

//將int 包裝成 4byte char array
void packint(int num, char *s)
{
    s[0] = num & 0xff;
    s[1] = (num>>8) & 0xff;
    s[2] = (num>>16) & 0xff;
    s[3] = (num>>24) & 0xff;
}

//將4byte char array 還原成 int
void unpackint(char *s, int &num)
{
    num = s[3] & 0xff;
    num = (num<<8) | (s[2] & 0xff);
    num = (num<<8) | (s[1] & 0xff);
    num = (num<<8) | (s[0] & 0xff);
}

//將packet 包裝成 char array
void pack(struct packet p, char *s)
{
    memset(s, 0, sizeof(char)*PACK_LEN);
    s[0] |= p.syn;
    s[0] = (s[0]<<1) | p.ack;
    s[0] = (s[0]<<1) | p.fin;
    packint(p.seq_n, s + 1);
    packint(p.ack_n, s + 5);
    packint(p.data, s + 9);
}

//將char array 還原成 packet
void unpack(char *s, struct packet &p)
{
    memset(&p, 0, sizeof(p));
    p.fin = s[0] & 1;
    p.ack = (s[0]>>1) & 1;
    p.syn = (s[0]>>2) & 1;
    unpackint(s + 1, p.seq_n);
    unpackint(s + 5, p.ack_n);
    unpackint(s + 9, p.data);
}


//設定封包內容
void setPacket(struct packet &p, bool syn, bool ack, bool fin, int seq_n, int ack_n, int data)
{
    memset(&p, 0, sizeof(p));
    p.syn = syn;p.ack = ack;p.fin = fin;
    p.seq_n = seq_n;
    p.ack_n = ack_n;
    p.data = data;
}

//檢查封包是否符合格式，若不檢查seq_n和ack_n，則設為-1。
bool checkPacket(const struct packet &p, bool syn, bool ack, bool fin, int seq_n, int ack_n)
{
	if((p.syn != syn) || (p.ack != ack) || (p.fin != fin))
		return 0;
	if(seq_n != -1 && p.seq_n != seq_n) return 0;
	if(ack_n != -1 && p.ack_n != ack_n) return 0;
	return 1;
}


struct timeval tv;
//set recv timeout = 0.1s
int setRecvTimeout(int fd)
{
    tv.tv_sec = 0; tv.tv_usec = 100000;
    return setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv));
}

//remove recv timeout
int rmRecvTimeout(int fd)
{
	tv.tv_sec = 0; tv.tv_usec = 0;
	return setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv));
}

//初始化class
SlideWindow::SlideWindow(int head_seq){
    head = 0;
    this->head_seq = head_seq;
    memset(received,0,sizeof(bool)*WINDOW_SIZE);
}

//加入收到的seq，回傳已收到的最小seq，並移動window
int SlideWindow::insert(int seq){
    if(seq == head_seq-1) return head_seq-1;
    if(seq < head_seq || seq > head_seq+WINDOW_SIZE-1)
        return -1;
    if(seq != head_seq){
        if(received[((seq-head_seq)+head)%WINDOW_SIZE] == 1) 
            return -1;
        received[((seq-head_seq)+head)%WINDOW_SIZE] = 1;
    }else{
        do{
            received[head] = 0;
            head = (head + 1) % WINDOW_SIZE;
            head_seq++;
        }while (received[head] == 1);
    }    
    return head_seq-1;
}
