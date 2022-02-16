#include <pcap.h>
#include "../functions/hacking.h"

void pcap_fatal(const char *failed_in, const char *errbuf) {
    printf("Fatal error: %s: %s\n", failed_in, errbuf);
    exit(1);
}

int main() {
    struct pcap_pkthdr header; // packet header
    const u_char *packet;
    char errbuf[PCAP_ERRBUF_SIZE]; // 256とpcap.hで定義
    char *device;
    pcap_t *pcap_handle; // ファイル記述子に似た
    int i;

    device = pcap_lookupdev(errbuf);
    if(device == NULL) pcap_fatal("pcap_lookupdev", errbuf);
    printf("sniff at: %s\n", device); // /dev/eth0など

    pcap_handle = pcap_open_live(device, 4096, 1, 0, errbuf); // パケットキャプチャデバイス
    if(pcap_handle == NULL) pcap_fatal("pcap_open_live", errbuf);

    for(i=0; i<5; i++) {
        packet = pcap_next(pcap_handle, &header); // パケット取得
        printf("got %d bytes of packet.\n", header.len);
        dump(packet, header.len);
    }
    pcap_close(pcap_handle);
}