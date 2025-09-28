#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <net/ethernet.h>  

static int total_packet_count = 0;
static int tcp_packet_count = 0;
static int filter_tcp_count = 0;

struct filters {
    int srcaddr_set;
    int dstaddr_set;
    int srcport_set;
    int dstport_set;
    struct in_addr ip_src;
    struct in_addr ip_dst;
    struct in6_addr ip6_src;
    struct in6_addr ip6_dst;
    uint16_t srcport;
    uint16_t dstport;
};

void packet_handler(u_char *user, const struct pcap_pkthdr *header, const u_char *bytes) {
    struct filters *f = (struct filters *)user;
    total_packet_count++;

    const struct ip *ip_hdr = (struct ip *)(bytes + 14)

    if (ip_hdr->ip_p != IPPROTO_TCP) return;
    tcp_packet_count ++;

    const struct tcphdr *tcp_hdr = (struct tcphdr *)((u_char *)ip_hdr + ip_hdr->ip_hl * 4);
    
    int f_mode = 1;
    if (f->srcaddr_set && ip_hdr->ip_src.s_addr != f->ip_src.s_addr) f_mode = 0;
    if (f->dstaddr_set && ip_hdr->ip_dst.s_addr != f->ip_dst.s_addr) f_mode = 0;
    if (f->srcport_set && ntohs(tcp_hdr->source) != f->srcport) f_mode = 0;
    if (f->dstport_set && ntohs(tcp_hdr->dest) != f->dstport) f_mode = 0;

    if (f_mode) filter_tcp_count++;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Specify the derictory to pcap-file and add filter [--scraddr ip] [--dstaddr ip] [--scrport ipport] [--dstport port]\n", argv[0]);
        return 1;
    }

    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(argv[1], error_buffer);
    if (!handle) {
        fprintf(stderr, "I can't open the file %s: %s\n", argv[1], error_buffer);
        return 1;
    }

    struct filters f = {0};
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--srcaddr") == 0 && i + 1 < argc) {
            if (inet_aton(argv[++i], &f.ip_src)) f.srcaddr_set = 1;
        }
         else if (strcmp(argv[i], "--dstaddr") == 0 && i + 1 < argc) {
            if (inet_aton(argv[++i], &f.ip_dst)) f.dstaddr_set = 1;
        }
        else if (strcmp(argv[i], "--srcport") == 0 && i + 1 < argc) {
            f.srcport = atoi(argv[++i]);
            f.srcport_set = 1;
        }
        else if (strcmp(argv[i], "--dstport") == 0 && i + 1 < argc) {
            f.dstport = atoi(argv[++i]);
            f.dstport_set = 1;
        }
    }

    pcap_loop(handle, 0, packet_handler, (u_char *)&f);
    pcap_close(handle);

    printf("Total packet count: %d\n", total_packet_count);
    printf("TCP packet count: %d\n", tcp_packet_count);
    printf("Filtered TCP count: %d\n", filter_tcp_count);
    return 0;
}
