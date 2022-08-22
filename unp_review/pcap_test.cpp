#ifdef __cplusplus
extern "C" {
#endif
#include <pcap.h>
#ifdef __cplusplus
}
#endif

void printer(u_char* user, const struct pcap_pkthdr *h, const u_char *p) {
    printf("I get one packet!");
}

#define DEFAULT_SNAPLEN 68
int main() {
    // ebuf, 错误信息的缓冲区，几乎每个pcap API都要传入ebuf
    char ebuf[PCAP_ERRBUF_SIZE];
    // 返回网络设备名，出错返回NULL设置ebuf
    char *device = pcap_lookupdev(ebuf);

    bpf_u_int32 local_net, netmask;
    pcap_lookupnet(device, &local_net, &netmask, ebuf);
    printf("%u.%u.%u.%u", local_net&0xff, local_net>>8&0xff, 
        local_net>>16&0xff, local_net>>24&0xff);
    printf(": %u.%u.%u.%u ", netmask&0xff, netmask>>8&0xff, 
        netmask>>16&0xff, netmask>>24&0xff);
    // device, snaplen, promisc. to_ms, ebuf
    pcap_t *pd = pcap_open_live(device, DEFAULT_SNAPLEN, 0, 1000, ebuf);

    if(pcap_datalink(pd) == DLT_EN10MB) {
        printf("10M ");
    }

    bpf_program fcode;
    pcap_compile(pd, &fcode, NULL, 1, 0);
    pcap_setfilter(pd, &fcode);
    pcap_loop(pd, 10, printer, NULL);

    pcap_stat stat;
    pcap_stats(pd, &stat);
    printf("recv: %d, drop: %d. \n", stat.ps_recv, stat.ps_drop);
    pcap_close(pd);
    return 0;
}