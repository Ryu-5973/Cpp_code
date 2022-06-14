#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

#define u_char unsigned char
#define DEFAULT_SNAPLEN 68

void printer(u_char * user, const struct pcap_pkthdr * h, const u_char * p) {
	printf("I get one packet!\n");
}

int main() {
	char ebuf[PCAP_ERRBUF_SIZE];

	// 这里会有个warning，pcap_lookupdev已经过时，建议用pcap_findalldevs，但是不影响运行
	// char *device = pcap_lookupdev(ebuf);
	// if (device==NULL) {
	// 	printf ("%s:%s\n","Cannot lookupdev",ebuf);
	// 	exit(1);
	// }else {
	//   	printf ("The device is :%s\n",device);
	// }
	
	pcap_if_t *allDevice;
	int ret = pcap_findalldevs(&allDevice, ebuf);
	// 返回值判断
	if(ret != 0) {
		printf ("%s:%s\n","Cannot lookupdev",ebuf);
		exit(1);
	}
	char *device = allDevice[0].name;
	if (device==NULL) {
		printf ("%s:%s\n","Cannot lookupdev",ebuf);
		exit(1);
	}else {	
	  	printf ("The device is :%s\n",device);
	}
	
	bpf_u_int32 localnet, netmask;
	printf("%s\n","The net:");
	if(pcap_lookupnet(device, &localnet, &netmask, ebuf) == 0) {
	  	printf("%u.%u.%u.%u", localnet&0xff, localnet>>8&0xff,
			localnet>>16&0xff, localnet>>24&0xff);
	  	printf(":%d.%d.%d.%d ", netmask&0xff, netmask>>8&0xff,
			netmask>>16&0xff, netmask>>24&0xff);
	}else {
	  	printf("look up net failure: %s\n",ebuf);
		// 报错完应该结束程序
		exit(1);
	}

	pcap_t *pd = pcap_open_live(device, DEFAULT_SNAPLEN, 0, 1000, ebuf);
	// 返回值判断
	if(pd == NULL) {
		printf("open live failure: %s\n", ebuf);
		exit(1);
	}

	if(pcap_datalink(pd) == DLT_EN10MB) {
		printf("10Mb Ethernet\n");
	}

	struct bpf_program fcode;
	ret = pcap_compile(pd, &fcode, NULL, 1, 0);
	// 返回值判断
	if(ret == -1) {
		printf("compile failure: %s\n", ebuf);
		exit(1);
	}
	ret = pcap_setfilter(pd, &fcode);
	// 返回值判断
	if(ret == -1) {
		printf("set filter failure: %s\n", ebuf);
		exit(1);
	}

	pcap_loop(pd, 10, printer, NULL);

	struct pcap_stat stat;
	pcap_stats(pd, &stat);
	printf("recv %d, drop %d. ", stat.ps_recv, stat.ps_drop);

	pcap_close(pd);
	exit(0);
}
