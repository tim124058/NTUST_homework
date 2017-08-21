/* compile : g++ -std=c++11 firewall.cpp -o firewall -lpcap */

#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <sstream>
using namespace std;

#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/if_ether.h>

#include <pcap.h>


/* TCP header */
/* ref : https://www.scss.tcd.ie/~feghhis/ta_project/webfetch_scripts/parse_tcpdump.c */
struct TCP_hdr {
        u_short th_sport;   /* source port */
        u_short th_dport;   /* destination port */
        u_int32_t th_seq;       /* sequence number */
        u_int32_t th_ack;       /* acknowledgement number */

        u_char th_offx2;    /* data offset, rsvd */
    #define TH_OFF(th)  (((th)->th_offx2 & 0xf0) >> 4)
        u_char th_flags;
    #define TH_FIN 0x01
    #define TH_SYN 0x02
    #define TH_RST 0x04
    #define TH_PUSH 0x08
    #define TH_ACK 0x10
    #define TH_URG 0x20
    #define TH_ECE 0x40
    #define TH_CWR 0x80
    #define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
        u_short th_win;     /* window */
        u_short th_sum;     /* checksum */
        u_short th_urp;     /* urgent pointer */
};

/* UDP header */
struct UDP_hdr {
	u_short	uh_sport;		/* source port */
	u_short	uh_dport;		/* destination port */
	u_short	uh_ulen;		/* datagram length */
	u_short	uh_sum;			/* datagram checksum */
};

//network byte order IP and PORT struct
struct IP_PORT{
	uint32_t s_addr;		// source address
	uint32_t d_addr;		// destination address
	u_short s_port;			// source port
	u_short d_port;			// destination port
};

/* Returns a string representation of a timestamp. */
const char *timestamp_string(struct timeval ts)
{
	static char timestamp_string_buf[256];
	sprintf(timestamp_string_buf, "%d.%06d",(int) ts.tv_sec, (int) ts.tv_usec);
	return timestamp_string_buf;
}

/* Report a problem with dumping the packet with the given timestamp. */
void problem_pkt(struct timeval ts, const char *reason)
{
	fprintf(stderr, "%s: %s\n", timestamp_string(ts), reason);
}

/* Report the specific problem of a packet being too short. */
void too_short(struct timeval ts, const char *truncated_hdr)
{
	fprintf(stderr, "packet with timestamp %s is truncated and lacks a full %s\n",
	timestamp_string(ts), truncated_hdr);
}

//設定pkIpPort的ip、port並回傳true，若不是TCP or UDP回傳false
bool setIP_PORT(const unsigned char *packet, struct timeval ts, unsigned int capture_len,struct IP_PORT *pkIpPort)
{
	bool isTCP = true;
	struct ip *ip;
	struct TCP_hdr *tcp;
	struct UDP_hdr *udp;
	unsigned int IP_header_length;

	/* For simplicity, we assume Ethernet encapsulation. */
	if (capture_len < sizeof(struct ether_header)){
		too_short(ts, "Ethernet header");
		return false;
	}

	/* Skip over the Ethernet header. */
	packet += sizeof(struct ether_header);
	capture_len -= sizeof(struct ether_header);

	if (capture_len < sizeof(struct ip))
	{ /* Didn't capture a full IP header */
		too_short(ts, "IP header");
		return false;
	}

	ip = (struct ip*) packet;
	IP_header_length = ip->ip_hl * 4;	/* ip_hl is in 4-byte words */

	if (capture_len < IP_header_length)
	{ /* didn't capture the full IP header including options */
		too_short(ts, "IP header with options");
		return false;
	}

	//設定IP下一層的協定(TCP or UDP)
	if (ip->ip_p == IPPROTO_TCP){
		isTCP = true;
	}else if(ip->ip_p == IPPROTO_UDP){
		isTCP = false;
	}else{
		problem_pkt(ts, "packet error");
		return false;
	}


	//set s_addr,d_addr
	memset(pkIpPort, 0, sizeof(*pkIpPort));
	pkIpPort->s_addr = ip->ip_src.s_addr;
	pkIpPort->d_addr = ip->ip_dst.s_addr;


	/* Skip over the IP header */
	packet += IP_header_length;
	capture_len -= IP_header_length;
	if(isTCP){
		if (capture_len < sizeof(struct TCP_hdr)){
			too_short(ts, "TCP header");
			return false;
		}
		tcp = (struct TCP_hdr*) packet;
		pkIpPort->s_port = tcp->th_sport;
		pkIpPort->d_port = tcp->th_dport;
	}else{
		if (capture_len < sizeof(struct UDP_hdr)){
			too_short(ts, "UDP header");
			return false;
		}
		udp = (struct UDP_hdr*) packet;
		pkIpPort->s_port = udp->uh_sport;
		pkIpPort->d_port = udp->uh_dport;
	}
	return true;
}

//去除前後的空白
string trim(std::string& str)
{
    str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
    str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
    return str;
}

//以空白切割字串
string split_space(string &s){
	if(trim(s)=="") return "";
	int space_pos = s.find(' ');
	if(space_pos == -1) space_pos = s.length();
	string splited = s.substr(0,space_pos);
    s = s.substr(space_pos);
    trim(s);
    return splited;
}

//檢查IP格式是否正確
bool isIP(string ip){
	istringstream iss(ip);
	string token;
    int c = 0;
    try{
	    while (getline(iss, token, '.')){
	    	c++;
	        if( stoi(token) > 255 || stoi(token) < 0 ) throw string("ip error");
	    }	
	    if(c != 4) throw string("ip error");
	    return true;
	}catch(...){
		return false;
	}
}
//檢查port格式是否正確
bool isPort(string port){
	try{
		int p = stoi(port);
		if ( p < 0 || p > 65535 ) throw string("port error");
		return true;
	}catch(...){
		return false;
	}
}

/*  在rules中新增一個格式為 : 
 *  <Source IP> <Dest IP> <Source port> <Dest port> 
 *  的rule，中間以空白分割，請勿使用tab
 */
bool addRule(vector<IP_PORT> &rules, string rule)
{
	IP_PORT r;
	string s_addr,d_addr,s_port,d_port;
	s_addr = split_space(rule);
	d_addr = split_space(rule);
	s_port = split_space(rule);
	d_port = split_space(rule);
	if(s_addr=="" || d_addr=="" || s_port=="" || d_port=="" || rule!="") return false;
	if(!isIP(s_addr) || !isIP(d_addr) || !isPort(s_port) | !isPort(d_port)) return false;
	memset(&r, 0, sizeof(r));
	r.s_addr = inet_addr(s_addr.c_str());
	r.d_addr = inet_addr(d_addr.c_str());
	r.s_port = htons(stoi(s_port));
	r.d_port = htons(stoi(d_port));
	rules.push_back(r);
	return true;
}

//如果pkIpPort在rules中return true
bool matchRule(const vector<IP_PORT> &rules,const IP_PORT &pkIpPort)
{
	for (int i = 0; i < rules.size(); i++){
		if(memcmp(&rules[i], &pkIpPort, sizeof(IP_PORT))==0)
			return true;
	}
	return false;
}

int main(int argc, char *argv[])
{
	pcap_t *pcap;						//讀取的pcap檔
	const unsigned char *packet;		//讀取出來的1個封包
	char errbuf[PCAP_ERRBUF_SIZE];
	struct pcap_pkthdr header;
	int pkt_count = 0;
	IP_PORT pkIpPort;					//從packet中取出來的IP和port
	vector<IP_PORT> rules;				//存放要drop的rule

	pcap_dumper_t *dumpfile;			//輸出的檔案

	if ( argc != 4 ){
		fprintf(stderr, "Usage : %s <rule.txt> <input.pcap> <output.pcap>\n", argv[0]);
		exit(1);
	}

	//open rule file and add rule
	ifstream ruleFd(argv[1]);
	string rule;
	if (!ruleFd.is_open()){
		fprintf(stderr, "error open rule file\n");
		exit(1);
	}
	while (getline (ruleFd,rule)) {
		if(!addRule(rules, rule))
			fprintf(stderr, "add rule \"%s\" error...\n", rule.c_str());
		else
			printf("add rule \"%s\" success!\n",rule.c_str());
	}
	ruleFd.close();	


	//open input pcap file
	pcap = pcap_open_offline(argv[2], errbuf);
	if (pcap == NULL){
		fprintf(stderr, "error reading pcap file: %s\n", errbuf);
		exit(1);
	}

	//open output pcap file
	dumpfile = pcap_dump_open(pcap, argv[3]);
	if(dumpfile==NULL){
    	fprintf(stderr,"Error opening output file\n");
    	exit(1);
	}

	//依序讀出每個packet
	while ((packet = pcap_next(pcap, &header)) != NULL){
		pkt_count++;
		//是TCP or UDP而且match rule就drop
		if(setIP_PORT(packet, header.ts, header.caplen, &pkIpPort)){
			if(matchRule(rules, pkIpPort)){
				printf("packet %d be dropped\n",pkt_count);
				continue;
			}
		}
		
		//寫入packet
		pcap_dump((unsigned char *) dumpfile, &header, packet);
	}
	return 0;
}
