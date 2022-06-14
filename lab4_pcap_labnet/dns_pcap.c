/*
 *
 *  libnet 1.1
 *  Build a DNSv4 packet
 *  To view: /usr/sbin/tcpdump -vvvvven -s 0 port 53
 *
 *  Copyright (c) 2003 Fr�d�ric Raynal <pappy@security-labs.org>
 *  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libnet.h>
#include <pcap/pcap.h>
#include <sys/types.h>
#include <sys/wait.h>

#define u_char 	unsigned char
#define u_long 	unsigned long
#define u_short unsigned short
#define DEFAULT_SNAPLEN 1500

uint8_t query_len;

void usage(char *prog) {
    fprintf(stderr, "Usage: %s -d dst_ip -q query_host [-s src_ip] [-t]\n", prog);
    exit(1);
}

void printnt_ip(const u_char *p) {
	unsigned long l=((*p) << 24) + (*(p+1)<< 16) + (*(p+2)<<8) + (*(p+3));
	struct in_addr addr;
	addr.s_addr=l;
	printf("ip: %s\n", inet_ntoa(addr));
}

void printer(u_char *user, const struct pcap_pkthdr *h, const u_char *p) {
	printf("I get one packet:%d\n",h->len);
	int dns_h=LIBNET_ETH_H+LIBNET_IPV4_H+LIBNET_UDP_H;
	int index=2;
	u_char ch1,ch2;
	unsigned short rrs;
	//flags: QR==0 or 1? 0:query;1:response
	ch1=*(p+dns_h+index);
	ch1=ch1 & 0x80;
	if(ch1==0) {
		printf("not a response.\n");
		return;
	}else {
		// char *pp = (char *)p;
		// printf("\n\npackage: \n");
		// for(int i = 0; i <500; ++ i) {
		// 	// printf("%d: %x(%c)\t\t", i, pp[i], pp[i]);
		// 	printf("%x: %d\t", i, pp[i]);
		// 	if(i % 4 == 0) printf("\n");
		// }
		// printf("\n");
		//get rrs number
		ch1=*(p+dns_h+6);
		ch2=*(p+dns_h+7);
		rrs=(ch1 <<8) + ch2;
		// 解析响应报文
		// Query: p+dns_h+12
		// Answer: p+dns_h+12+query_len
		// int i;
		const u_char *answer = p + dns_h + 12 + query_len;
		int base = 2 + 2 + 2 + 4 + 2; // name+type+class+ttl+len
		for(int i = 0; i < rrs; ++ i) {
			const u_char *cur = answer;
			int type = (cur[2] << 8) + cur[3];
			int len = (cur[10] << 8) + cur[11];
			// printf("type: %d len: %d\n", type, len);
			if(type == 1) {	// IPv4
				u_char reverse[4];
				for(int j = 0; j < 4; ++ j) {
					reverse[j] = cur[base + 3 - j];
				}
				printnt_ip(reverse);
			}
			answer += (base + len);

			// printf("\n");
			// printnt_ip(reverse);
		}
		printf ("rrs:%d\n",rrs);
	}
}

int get_dns_result() {
  	char ebuf[PCAP_ERRBUF_SIZE];
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
	// char *device = pcap_lookupdev(ebuf);
	struct bpf_program fcode;
	int iret;
	pcap_t *pd = pcap_open_live(device, DEFAULT_SNAPLEN, 0, 1000, ebuf);
	
	iret=pcap_compile(pd, &fcode, "udp port 53", 1, 0);
	if(iret==-1) {
	  	printf("%s\n","pcap_compile failed.");
	  	exit(1);
	}
	pcap_setfilter(pd, &fcode);

	pcap_loop(pd, 2, printer, NULL);
	pcap_close(pd);
	return 0;
} 

int dns_name(const char *dnsstr, char *dnsname) {
	int count_index=0, index=1,i=0;
	char count=0, ch;

	while((ch=*(dnsstr+i)) != 0){
		if(ch=='.'){
			dnsname[count_index]=count;
			count_index=index++;
			count=0;
		}else{
			dnsname[index++]=ch;
			count++;
		}
		i++;
	}
	dnsname[count_index]=count;
	dnsname[index++]='\0';
	return strlen(dnsname);
}

int main(int argc, char *argv[]) {
    char c;
    u_long src_ip = 0, dst_ip = 0;
    u_short type = LIBNET_UDP_DNSV4_H;
    libnet_t *l;

    libnet_ptag_t ip;
    libnet_ptag_t ptag4; /* TCP or UDP ptag */
    libnet_ptag_t dns;
    
    char errbuf[LIBNET_ERRBUF_SIZE];
    char *query = NULL;
    char payload[1024];
    u_short payload_s;

    printf("libnet 1.1 packet shaping: DNSv4[raw]\n");
    /*
     *  Initialize the library.  Root priviledges are required.
     */
    l = libnet_init(
            LIBNET_RAW4,                            /* injection type */
            NULL,                                   /* network interface */
            errbuf);                                /* error buffer */
  
    if (!l) {
        fprintf(stderr, "libnet_init: %s", errbuf);
        exit(EXIT_FAILURE);
    }

	/*
     * parse options
     */
    while ((c = getopt(argc, argv, "d:s:q:t")) != EOF) {
        switch (c) {
            case 'd':
                if ((dst_ip = libnet_name2addr4(l, optarg, LIBNET_RESOLVE)) == -1) {
                    fprintf(stderr, "Bad destination IP address: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case 's':
                if ((src_ip = libnet_name2addr4(l, optarg, LIBNET_RESOLVE)) == -1) {
                    fprintf(stderr, "Bad source IP address: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'q':
                query = optarg;
                break;
            case 't':
                type = LIBNET_TCP_DNSV4_H;
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }
    
    if (!src_ip){
        src_ip = libnet_get_ipaddr4(l);
    }

    if (!dst_ip  || !query) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }


    /* 
     * build dns payload 
     */
	/* int i,slen;
    char *s=dns2str(query);
    slen=strlen(s);
    //ss=s;
	printf("length(%d):%s\n",slen,s );
    for(i=0;i<slen;i++)
      printf ("%2x\n",*s++);
    payload_s = snprintf(payload, sizeof payload, "%s%c%c%c%c%c", 
	s, 0x00, 0x00, 0x01, 0x00, 0x01);*/
	//query name: a string ended with 0x00
	//query type: 16 bit, 0x00 0x01
	//query class: 16 bit, 0x00 0x01
    
	char ss[256];
	int ret;
	printf("query str: %s\n",query);
	ret=dns_name(query,ss);
	printf("length(%d):%s\n",ret, ss);
    // for(i=0;i<ret;i++) {
	// 	printf ("%2x\n",ss[i]);
	// }
	payload_s = snprintf(payload, sizeof payload, "%s%c%c%c%c%c", 
			ss, 0x00, 0x00, 0x01, 0x00, 0x01);
	query_len = payload_s;
    /* 
     * build packet
     */
    dns = libnet_build_dnsv4(
		type,          /* TCP or UDP */
		0x7777,        /* id */
		0x0100,        /* request */
		1,             /* num_q */
		0,             /* num_anws_rr */
		0,             /* num_auth_rr */
		0,             /* num_addi_rr */
		(uint8_t*)payload,
		payload_s,
		l,
		0
	);
   
    if (dns == -1) {
        fprintf(stderr, "Can't build  DNS packet: %s\n", libnet_geterror(l));
        goto bad;
    }

    if (type == LIBNET_TCP_DNSV4_H) /* TCP DNS */ {
		ptag4 = libnet_build_tcp(
			0x6666,                                    /* source port */
			53,                                        /* destination port */
			0x01010101,                                /* sequence number */
			0x02020202,                                /* acknowledgement num */
			TH_PUSH|TH_ACK,                            /* control flags */
			32767,                                     /* window size */
			0,                                         /* checksum */
			0,                                         /* urgent pointer */
			LIBNET_TCP_H + LIBNET_TCP_DNSV4_H + payload_s, /* TCP packet size */
			NULL,                                      /* payload */
			0,                                         /* payload size */
			l,                                         /* libnet handle */
			0										   /* libnet id */
		);                                        
		
		if (ptag4 == -1) {
			fprintf(stderr, "Can't build UDP header: %s\n", libnet_geterror(l));
			goto bad;
		}
		
		
		ip = libnet_build_ipv4(
			LIBNET_IPV4_H + LIBNET_TCP_H + type + payload_s,/* length */
			0,                                          /* TOS */
			242,                                        /* IP ID */
			0,                                          /* IP Frag */
			64,                                         /* TTL */
			IPPROTO_TCP,                                /* protocol */
			0,                                          /* checksum */
			src_ip,                                     /* source IP */
			dst_ip,                                     /* destination IP */
			NULL,                                       /* payload */
			0,                                          /* payload size */
			l,                                          /* libnet handle */
			0                                           /* libnet id */
		);
		if (ip == -1) {
			fprintf(stderr, "Can't build IP header: %s\n", libnet_geterror(l));
			exit(EXIT_FAILURE);
		}
    }else { /* UDP DNS */
        ptag4 = libnet_build_udp(
            0x6666,                                /* source port */
            53,                                    /* destination port */
            LIBNET_UDP_H + LIBNET_UDP_DNSV4_H + payload_s, /* packet length */
            0,                                      /* checksum */
            NULL,                                   /* payload */
            0,                                      /* payload size */
            l,                                      /* libnet handle */
            0                                       /* libnet id */
		);
	}
	if (ptag4 == -1) {
	    fprintf(stderr, "Can't build UDP header: %s\n", libnet_geterror(l));
	    goto bad;
	}

	
	ip = libnet_build_ipv4(
	    LIBNET_IPV4_H + LIBNET_UDP_H + type + payload_s,/* length */
	    0,                                          /* TOS */
	    242,                                        /* IP ID */
	    0,                                          /* IP Frag */
	    64,                                         /* TTL */
	    IPPROTO_UDP,                                /* protocol */
	    0,                                          /* checksum */
	    src_ip,                                     /* source IP */
	    dst_ip,                                     /* destination IP */
	    NULL,                                       /* payload */
	    0,                                          /* payload size */
	    l,                                          /* libnet handle */
	    0                                           /* libnet id */
	);
	if (ip == -1) {
	    fprintf(stderr, "Can't build IP header: %s\n", libnet_geterror(l));
	    exit(EXIT_FAILURE);
	}

	//run pcap in another process
	int iret;
	if((iret=fork())==0) {
	  get_dns_result();
	}else{
	  
	  
		//wait 3 seconds, pcap would run in another process
		sleep(3);
		/* write to the wire  */
		c = libnet_write(l);
		if (c == -1) {
			fprintf(stderr, "Write error: %s\n", libnet_geterror(l));
			goto bad;
		} else {
			fprintf(stderr, "Wrote %d byte DNS packet; check the wire.\n", c);
		}
		libnet_destroy(l);
		int status;
		wait(&status);
		return (EXIT_SUCCESS);
	bad:
		libnet_destroy(l);
		return (EXIT_FAILURE);
	}
	return 0;
}
