#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include "../functions/hacking.h"
#include "../functions/hacking-network.h"

void pcap_fatal(const char *, const char *);
void decode_ethernet(const u_char *);
void decode_ip(const u_char *);
u_int decode_tcp(const u_char *);

void caught_packet(u_char *, const struct pcap_pkthdr *, const u_char *);

int main() {
    struct pcap_pkthdr cap_header; // packet header
    const u_char *packet, *pkt_data;
    char errbuf[PCAP_ERRBUF_SIZE]; // 256とpcap.hで定義
    char *device;
    pcap_t *pcap_handle; // ファイル記述子に似た

    device = pcap_lookupdev(errbuf);
    if(device == NULL) pcap_fatal("pcap_lookupdev", errbuf);
    printf("sniff at: %s\n", device); // /dev/eth0など

    pcap_handle = pcap_open_live(device, 4096, 1, 0, errbuf); // パケットキャプチャデバイス
    if(pcap_handle == NULL) pcap_fatal("pcap_open_live", errbuf);

    pcap_loop(pcap_handle, 3, caught_packet, NULL); // パケットをキャプチャするたびに第3引数の指すコールバック関数を呼び出す。
    pcap_close(pcap_handle);
}

void caught_packet(u_char *user_args, const struct pcap_pkthdr *cap_header, const u_char *packet) {
    int tcp_header_length, total_header_size, pkt_data_len;
    u_char *pkt_data;

    printf("==== Got %d bytes of packet ====\n", cap_header->len);

    decode_ethernet(packet);
    decode_ip(packet+ETHER_HDR_LEN);
    tcp_header_length = decode_tcp(packet+ETHER_HDR_LEN+sizeof(struct ip_hdr));

    total_header_size = ETHER_HDR_LEN+sizeof(struct ip_hdr)+tcp_header_length;
    pkt_data = (u_char *)packet + total_header_size;
    pkt_data_len = cap_header->len - total_header_size;
    if(pkt_data_len > 0) {
        printf("\t\t\t%u bytes of packet data\n", pkt_data_len);
        dump(pkt_data, pkt_data_len);
    }
    else printf("\t\t\t no packet data\n");
}

void pcap_fatal(const char *failed_in, const char *errbuf) {
    printf("Fatal error: %s: %s\n", failed_in, errbuf);
    exit(1);
}

void decode_ethernet(const u_char *header_start) {
    int i;
    const struct ether_hdr *ethernet_header;

    ethernet_header = (const struct ether_hdr *)header_start;
    printf("[[  2nd layer:: ethernet header  ]]\n");
    printf("[ source: %02x", ethernet_header->ether_src_addr[0]);
    for(i=1; i < ETHER_ADDR_LEN; i++) printf(":%02x", ethernet_header->ether_src_addr[i]);

    printf("\tdestination: %02x", ethernet_header->ether_dest_addr[0]);
    for(i=1; i < ETHER_ADDR_LEN; i++) printf(":%02x", ethernet_header->ether_dest_addr[i]);
    printf("\ttype: %hu ]\n", ethernet_header->ether_type);
}

void decode_ip(const u_char *header_start) {
    const struct ip_hdr *ip_header;

    ip_header = (const struct ip_hdr *)header_start;
    printf("\t((  3rd layer::: IP header  ))\n");
    struct in_addr srcaddr = { htonl(ip_header->ip_src_addr) };
    printf("\t( source: %s\t", inet_ntoa(srcaddr));
    struct in_addr destaddr = { htonl(ip_header->ip_dest_addr) };
    printf("destination: %s )\n", inet_ntoa(destaddr));
    printf("\t( type: %u\t", (u_int) ip_header->ip_type);
    printf("ID: %hu\tlength: %hu )\n", ntohs(ip_header->ip_id), ntohs(ip_header->ip_len));
}

u_int decode_tcp(const u_char *header_start) {
    u_int header_size;
    const struct tcp_hdr *tcp_header;

    tcp_header = (const struct tcp_hdr *)header_start;
    header_size = 4 * tcp_header->tcp_offset;

    printf("\t\t{{  4th layer:::: TCP header  }}\n");
    printf("\t\t{ source port: %hu\t", ntohs(tcp_header->tcp_src_port));
    printf("destination port: %hu }\n", ntohs(tcp_header->tcp_dest_port));
    printf("\t\t{ Seq #: %u\t", ntohl(tcp_header->tcp_seq));
    printf("Ack #: %u }\n", ntohl(tcp_header->tcp_ack));
    printf("\t\t{ header size: %u\tflag: ", header_size);
    if(tcp_header->tcp_flags & TCP_FIN) printf("FIN ");
    if(tcp_header->tcp_flags & TCP_SYN) printf("SYN ");
    if(tcp_header->tcp_flags & TCP_RST) printf("RST ");
    if(tcp_header->tcp_flags & TCP_PUSH) printf("PUSH ");
    if(tcp_header->tcp_flags & TCP_ACK) printf("ACK ");
    if(tcp_header->tcp_flags & TCP_URG) printf("URG ");
    printf(" }\n");

    return header_size;
}