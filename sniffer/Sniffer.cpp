//
// Created by Bogdan on 07.04.18.
//

#include <sstream>
#include "Sniffer.h"

#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>

#define SNAP_LEN 65536


#define SIZE_ETHERNET 14


#define ETHER_ADDR_LEN  6
#define PCAP_SAVEFILE "./pcap_savefile"

#define IFSZ 16


struct sniff_ethernet {
    u_char  ether_dhost[ETHER_ADDR_LEN];    
    u_char  ether_shost[ETHER_ADDR_LEN];    
    u_short ether_type;                     
};


struct sniff_ip {
    u_char  ip_vhl;                 
    u_char  ip_tos;                
    u_short ip_len;                 
    u_short ip_id;                  
    u_short ip_off;                 
#define IP_RF 0x8000            
#define IP_DF 0x4000            
#define IP_MF 0x2000            
#define IP_OFFMASK 0x1fff       
    u_char  ip_ttl;               
    u_char  ip_p;                   
    u_short ip_sum;                 
    struct  in_addr ip_src,ip_dst;  
};
#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)                (((ip)->ip_vhl) >> 4)


typedef u_int tcp_seq;

struct sniff_tcp {
    u_short th_sport;               
    u_short th_dport;               
    tcp_seq th_seq;                 
    tcp_seq th_ack;                 
    u_char  th_offx2;               
#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)
    u_char  th_flags;
#define TH_FIN  0x01
#define TH_SYN  0x02
#define TH_RST  0x04
#define TH_PUSH 0x08
#define TH_ACK  0x10
#define TH_URG  0x20
#define TH_ECE  0x40
#define TH_CWR  0x80
#define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
    u_short th_win;                 
    u_short th_sum;                 
    u_short th_urp;                
};


static std::string hostString;
static std::string mode_type;
static const char* fullHostAddress;
static char buffer[26];
static bool isHostFound = false;
static std::map<std::string,int> upSize;
static std::map<std::string,int> downSize;
static std::string hostPrefix;
static int readSec = 0;

using namespace std;

Sniffer::Sniffer(){
    this->num_packets = 10;
    init();
}
Sniffer::Sniffer(int num_packets){
    this->num_packets = num_packets;
    init();
}
void Sniffer::init() {
    int status = pcap_findalldevs(&alldevs, errbuf);
    if (status != 0) {
        printf("%s\n", errbuf);
        return;
    }
    if (!isHostFound) {
        bool flag = false;
        char *tmpAdr;
        for (pcap_if_t *d = alldevs; d != NULL; d = d->next) {
            printf("%s:", d->name);
            for (pcap_addr_t *a = d->addresses; a != NULL; a = a->next) {
                if (a->addr->sa_family == AF_INET) {
                    fullHostAddress = inet_ntoa(((struct sockaddr_in *) a->addr)->sin_addr);
                    printf(" %s", fullHostAddress);
                    hostString = string(fullHostAddress);
                    flag = true;
                    isHostFound = true;
                    break;
                }
            }
            if (flag)
                break;
        }
    }
    hostPrefix = split(fullHostAddress);
    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    puts(buffer);
             
    char filter_exp[] = "ip";
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n",
                errbuf);
    }
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n",
                dev, errbuf);
        net = 0;
        mask = 0;
    }

    printf("Device: %s\n", dev);
    printf("Number of packets: %d\n", num_packets);
    printf("Filter expression: %s\n", filter_exp);


    handle = pcap_open_live(dev, SNAP_LEN, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
    }

    if (pcap_datalink(handle) != DLT_EN10MB) {
        fprintf(stderr, "%s is not an Ethernet\n", dev);
    }

    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n",
                filter_exp, pcap_geterr(handle));
    }

    
    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n",
                filter_exp, pcap_geterr(handle));
    }

}

void Sniffer::sniffNetwork() {
        mode_type == "net";
        pcap_loop(handle, num_packets, this->pkt_callback, NULL);
        pcap_freecode(&fp);
        pcap_close(handle);
        getchar();
        for(auto elem : upSize)
        {
        std::cout << elem.first << " " << elem.second<<"\n";
        }
        for(auto elem : downSize)
        {
        std::cout << elem.first << " " << elem.second<<"\n";
        }
        plotResults(upSize,"uplink");
        plotResults(downSize,"downlink");

}
void Sniffer::sniffNetworkAndWrite() {
    mode_type == "writing";
    if ((pd = pcap_dump_open(handle, PCAP_SAVEFILE)) == NULL) {
        
        fprintf(stderr,
                "Error opening savefile \"%s\" for writing: %s\n",
                PCAP_SAVEFILE, pcap_geterr(handle));

    }

    if ((pcount = pcap_dispatch(handle, num_packets, &pcap_dump, (u_char *) pd)) < 0) {
        
        pcap_perror(handle, prestr);
    }
    printf("Packets received and successfully passed through filter: %d.\n",
           pcount);



    
    if (pcap_stats(handle, &ps) != 0) {
        fprintf(stderr, "Error getting Packet Capture stats: %s\n",
                pcap_geterr(handle));
    }

    printf("Packet Capture Statistics:\n");
    printf("%d packets received by filter\n", ps.ps_recv);
    printf("%d packets dropped by kernel\n", ps.ps_drop);
    pcap_dump_close(pd);
    pcap_freecode(&fp);
    pcap_close(handle);

}

void Sniffer::sniffFromFile() {
    pcap_t *descr = pcap_open_offline(PCAP_SAVEFILE, errbuf);
    if (descr == NULL) {
        cout << "pcap_open_live() failed: " << errbuf << endl;
    }

    if (pcap_loop(descr, num_packets, this->pkt_callback, NULL) < 0) {
        cout << "pcap_loop() failed: " << pcap_geterr(descr);
    }
    pcap_freealldevs(alldevs);
}

void Sniffer::gotPacket(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{

    static int count = 1;                   

    const struct sniff_ethernet *ethernet;  
    const struct sniff_ip *ip;              
    const struct sniff_tcp *tcp;            
    const char *payload;                    
    pcap_dumper_t *pd;
    int size_ip;
    int size_tcp;
    int size_payload;

    printf("\nPacket number %d:\n", count);
    count++;

    
    ethernet = (struct sniff_ethernet*)(packet);

    
    ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
    size_ip = IP_HL(ip)*4;
    if (size_ip < 20) {
        printf("   * Invalid IP header length: %u bytes\n", size_ip);
        return;
    }

    
    printf("       From: %s\n", inet_ntoa(ip->ip_src));
    printf("         To: %s\n", inet_ntoa(ip->ip_dst));
    string src = string((const char*) inet_ntoa(ip->ip_src));
    string dst = string((const char*)inet_ntoa(ip->ip_dst));
    
    switch(ip->ip_p) {
        case IPPROTO_TCP:
            printf("   Protocol: TCP\n");
            break;
        case IPPROTO_UDP:
            printf("   Protocol: UDP\n");
            break;
        case IPPROTO_ICMP:
            printf("   Protocol: ICMP\n");
            return;
        case IPPROTO_IP:
            printf("   Protocol: IP\n");
            return;
        default:
            printf("   Protocol: unknown\n");
            return;
    }

    tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
    size_tcp = TH_OFF(tcp)*4;
    /*if (size_tcp < 20) {
        printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
        return;
    }*/

    printf("   Src port: %d\n", ntohs(tcp->th_sport));
    printf("   Dst port: %d\n", ntohs(tcp->th_dport));

    std::cout<<mode_type<<endl;
    
    /*payload = (const char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);

    
    size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);*/
    char timeArray[20];
    
    if(mode_type == "net") {
        time_t timer;
        struct tm *tm_info;
        time(&timer);
        tm_info = localtime(&timer);
        strftime(timeArray, 26, "%S", tm_info);
    }/*else{
        strftime(timeArray, 26, "%S", localtime(&header->ts.tv_sec + readSec) );
    }*/
    //std::cout<<timeArray<<endl;
    int size = header->len;
    if(split(src) == hostPrefix){
        upSize.insert(std::pair<string,int>(string(timeArray),size));//ntohs(ip->ip_len) - (size_ip + size_tcp)));
        downSize.insert(std::pair<string,int>(string(timeArray),0));//ntohs(ip->ip_len) - (size_ip + size_tcp)));
        std::cout<<"uplink"<<endl;
    } else if(split(dst) == hostPrefix){
        downSize.insert(std::pair<string,int>(string(timeArray),size));//ntohs(ip->ip_len) - (size_ip + size_tcp)));
        upSize.insert(std::pair<string,int>(string(timeArray),0));
        std::cout<<"downlink"<<endl;
    }
}

void Sniffer::plotResults(map<string,int> data, string link) {
    FILE *gnuplotPipe, *tempDataFile;
    const char *tempDataFileName;
    tempDataFileName = link.c_str();
    gnuplotPipe = popen("gnuplot -persist", "w");
    fprintf(gnuplotPipe, "set term qt\n");

    if (gnuplotPipe) {
        fprintf(gnuplotPipe, "plot \"%s\" with linespoints ls 1\n", tempDataFileName);
        fflush(gnuplotPipe);

        tempDataFile = fopen(tempDataFileName, "w");
        auto it = data.begin();
        for (; it != data.end(); ++it) {
            fprintf(tempDataFile, "%s %ld\n", (it->first).c_str(), (long) it->second);
        }
        fclose(tempDataFile);


        printf("press enter to continue...");
        getchar();

        remove(tempDataFileName);

        fprintf(gnuplotPipe, "exit \n");
    } else {
        printf("gnuplot not found...");
    }
}
string Sniffer::split(string address) {
    string result;
    std::string token;
    std::stringstream line(address);
    int i = 0;
    while(i<3)
    {
        std::getline(line, token, '.');
        result.append(token);
        i++;
        if(i<3){
            result.append(".");
        }
    }
    return result;
}
