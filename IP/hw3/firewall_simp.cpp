#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <pcap.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/* UDP header */
struct UDP_hdr {
	u_short	uh_sport;		/* source port */
	u_short	uh_dport;		/* destination port */
	u_short	uh_ulen;		/* datagram length */
	u_short	uh_sum;			/* datagram checksum */
};

/* TCP header */
struct tcp_hdr {
        u_short th_sport;   /* source port */
        u_short th_dport;   /* destination port */
        u_int32_t th_seq;   /* sequence number */
        u_int32_t th_ack;   /* acknowledgement number */

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

//將ip和port輸出到info，成功回傳1
bool dump_packet(const unsigned char *packet, unsigned int capture_len, string &info)
{
	struct ip *ip;
	struct UDP_hdr *udp;
	struct tcp_hdr *tcp;
	unsigned int IP_header_length;
	char tmp[30];

	/* For simplicity, we assume Ethernet encapsulation. */
	if (capture_len < sizeof(struct ether_header)) return 0;

	/* Skip over the Ethernet header. */
	packet += sizeof(struct ether_header);
	capture_len -= sizeof(struct ether_header);

	/* Didn't capture a full IP header */
	if (capture_len < sizeof(struct ip)) return 0;

	ip = (struct ip*) packet;
	IP_header_length = ip->ip_hl * 4;	/* ip_hl is in 4-byte words */

	/* didn't capture the full IP header including options */
	if (capture_len < IP_header_length) return 0;
	int protocol = 0;	// 0->TCP 1->UDP
	if (ip->ip_p == IPPROTO_UDP) protocol = 1;

	info = "";
	inet_ntop(AF_INET, &ip->ip_src.s_addr, tmp, sizeof(tmp));
	info+=tmp; info+=" ";
	inet_ntop(AF_INET, &ip->ip_dst.s_addr, tmp, sizeof(tmp));
	info+=tmp; info+=" ";

	packet += IP_header_length;
	capture_len -= IP_header_length;

	if (capture_len < ((protocol)?sizeof(struct UDP_hdr):sizeof(struct tcp_hdr)) ) 
		return 0;

	if(protocol){
		udp = (struct UDP_hdr*) packet;
		sprintf(tmp,"%d %d", ntohs(udp->uh_sport), ntohs(udp->uh_dport));
	}else{
		tcp = (struct tcp_hdr*) packet;
		sprintf(tmp,"%d %d", ntohs(tcp->th_sport), ntohs(tcp->th_dport));
	}
	info+=tmp;
	return 1;
}

//info在rule中
bool inRule(vector<string> rules,string info){
	for (int i = 0; i < rules.size(); i++)
		if(rules[i] == info) return true;
	return false;
}

int main(int argc, char *argv[])
{
	pcap_t *pcap;
	const unsigned char *packet;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct pcap_pkthdr header;

	vector<string> rules;		//all rule
	string rule;
	pcap_dumper_t *dumpfile;

	/* Skip over the program name. */
	++argv; --argc;

	if ( argc != 3 ){
		fprintf(stderr, "program requires three argument, the trace file to dump\n");
		return -1;
	}

	//rule
	ifstream f(argv[0]);
	if (!f.is_open()){
		fprintf(stderr, "open rule file error\n");
		return -1;
	}
	while (getline(f,rule))
		if(rule != "")
			rules.push_back(rule);
	f.close();

	//input
	pcap = pcap_open_offline(argv[1], errbuf);
	if (pcap == NULL){
		fprintf(stderr, "error reading pcap file: %s\n", errbuf);
		return -1;
	}

	//output
	dumpfile = pcap_dump_open(pcap, argv[2]);
	if(dumpfile==NULL){
    	fprintf(stderr,"\nError opening output file\n");
    	return -1;
	}

	string info;
	printf("please wait...\n");
	while ((packet = pcap_next(pcap, &header)) != NULL){
		if(dump_packet(packet, header.caplen, info))
			if(inRule(rules,info)) continue;
		pcap_dump((unsigned char *) dumpfile, &header, packet);
	}
	printf("Done!!!\n");
	return 0;
}

//編譯 : g++ firewall_simp.cpp -o firewall -lpcap
//執行 : ./firewall rule.txt trace.pcap out.pcap
