//
// Created by Bogdan on 07.04.18.
//


#include <pcap.h>
#include <map>

class Sniffer {
private:

    time_t timer;
    
    struct tm *tm_info;
    char *dev = NULL;            
    
    pcap_t *handle;                
           
    struct bpf_program fp;            
    bpf_u_int32 mask;            
    bpf_u_int32 net;            
    int num_packets;
    pcap_dumper_t *pd;
    int pcount = 0;
    char prestr[80];
    int linktype = 0;
    struct pcap_stat ps;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs;
    
    

    void init();
public:
    Sniffer();
    Sniffer(int num_packets);
    void sniffNetwork();
    void sniffNetworkAndWrite();
    void sniffFromFile();
    void plotResults(std::map<std::string,int> data, std::string link);
    void gotPacket(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
    static void pkt_callback(u_char *user, const pcap_pkthdr *hdr, const u_char *bytes){
        Sniffer *sniffer=reinterpret_cast<Sniffer *>(user);
        sniffer->gotPacket(user, hdr, bytes);
    }
    std::string split(std::string address);

};
