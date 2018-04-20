//
// Created by Bogdan on 07.04.18.
//

#include <sstream>
#include "Sniffer.h"

static string hostString;
static string mode_type;
static const char* fullHostAddress;
static char buffer[26];
static bool isHostFound = false;
static std::map<string,int> upSize;
static std::map<string,int> downSize;
static string hostPrefix;
static int readSec = 0;
Sniffer::Sniffer(char mode) {
    if(mode == 'n'){
        mode_type = "net";
    }
    if(mode =='r'){
        mode_type = "read";
    }
    if(mode == 'w'){
        mode_type = "write";
    }
}
void Sniffer::init(char mode) {

}
int Sniffer::networkSniffer() {
    char c = getchar();
    char errbuf[PCAP_ERRBUF_SIZE];
    int num_packets = 50;
    pcap_if_t *alldevs;
    int status = pcap_findalldevs(&alldevs, errbuf);
    if (status != 0) {
        printf("%s\n", errbuf);
        return 1;
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

    pcap_dumper_t *pd;
    int pcount = 0;
    char prestr[80];
    int linktype = 0;
    struct pcap_stat ps;

    while (c!='q') {
        time_t timer;
        //char buffer[26];
        struct tm* tm_info;

        time(&timer);
        tm_info = localtime(&timer);

        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
        puts(buffer);

        char *dev = NULL;            /* capture device name */
            /* error buffer */
        pcap_t *handle;                /* packet capture handle */

        char filter_exp[] = "ip";        /* filter expression [3] */
        struct bpf_program fp;            /* compiled filter program (expression) */
        bpf_u_int32 mask;            /* subnet mask */
        bpf_u_int32 net;            /* ip */
        int num_packets = 70;            /* number of packets to capture */


            dev = pcap_lookupdev(errbuf);
            if (dev == NULL) {
                fprintf(stderr, "Couldn't find default device: %s\n",
                        errbuf);
                exit(EXIT_FAILURE);
            }
            /* get network number and mask associated with capture device */
            if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
                fprintf(stderr, "Couldn't get netmask for device %s: %s\n",
                        dev, errbuf);
                net = 0;
                mask = 0;
            }

            /* print capture info */
            printf("Device: %s\n", dev);
            printf("Number of packets: %d\n", num_packets);
            printf("Filter expression: %s\n", filter_exp);


            handle = pcap_open_live(dev, SNAP_LEN, 1, 1000, errbuf);
            if (handle == NULL) {
                fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
                exit(EXIT_FAILURE);
            }

            /* make sure we're capturing on an Ethernet device [2] */
            if (pcap_datalink(handle) != DLT_EN10MB) {
                fprintf(stderr, "%s is not an Ethernet\n", dev);
                exit(EXIT_FAILURE);
            }

            /* compile the filter expression */
            if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
                fprintf(stderr, "Couldn't parse filter %s: %s\n",
                        filter_exp, pcap_geterr(handle));
                exit(EXIT_FAILURE);
            }

            /* apply the compiled filter */
            if (pcap_setfilter(handle, &fp) == -1) {
                fprintf(stderr, "Couldn't install filter %s: %s\n",
                        filter_exp, pcap_geterr(handle));
                exit(EXIT_FAILURE);
            }


        if(mode_type == "net") {
            pcap_loop(handle, num_packets, this->pkt_callback, NULL);
            c = getchar();

            pcap_freecode(&fp);
            pcap_close(handle);
        }else if(mode_type == "write") {
            if ((pd = pcap_dump_open(handle, PCAP_SAVEFILE)) == NULL) {
                /*
                 * Print out error message if pcap_dump_open failed. This will
                 * be the below message followed by the pcap library error text,
                 * obtained by pcap_geterr().
                 */
                fprintf(stderr,
                        "Error opening savefile \"%s\" for writing: %s\n",
                        PCAP_SAVEFILE, pcap_geterr(handle));
                exit(7);
            }

            if ((pcount = pcap_dispatch(handle, num_packets, &pcap_dump, (u_char *) pd)) < 0) {
                /*
                 * Print out appropriate text, followed by the error message
                 * generated by the packet capture library.
                 */
                pcap_perror(handle, prestr);
                exit(8);
            }
            printf("Packets received and successfully passed through filter: %d.\n",
                   pcount);

            /*
             * Get and print the link layer type for the packet capture device,
             * which is the network device selected for packet capture.
             */
            if (!(linktype = pcap_datalink(handle))) {

                exit(9);
            }

            /*
             * Get the packet capture statistics associated with this packet
             * capture device. The values represent packet statistics from the time
             * pcap_open_live() was called up until this call.
             */
            if (pcap_stats(handle, &ps) != 0) {
                fprintf(stderr, "Error getting Packet Capture stats: %s\n",
                        pcap_geterr(handle));
                exit(10);
            }

            /* Print the statistics out */
            printf("Packet Capture Statistics:\n");
            printf("%d packets received by filter\n", ps.ps_recv);
            printf("%d packets dropped by kernel\n", ps.ps_drop);
            pcap_dump_close(pd);
            pcap_freecode(&fp);
            pcap_close(handle);

        }
        if(mode_type != "net") {
            pcap_t *descr = pcap_open_offline(PCAP_SAVEFILE, errbuf);
            if (descr == NULL) {
                cout << "pcap_open_live() failed: " << errbuf << endl;
                return 1;
            }

            if (pcap_loop(descr, num_packets, this->pkt_callback, NULL) < 0) {
                cout << "pcap_loop() failed: " << pcap_geterr(descr);
                return 1;
            }
        }


        printf("\nCapture complete.\n");
        c = getchar();
        plotResults(upSize, "upLink");
        plotResults(downSize, "downLink");
        std::cout<<"Uplink"<<endl;
        /*for(auto it = upSize.cbegin(); it != upSize.cend(); ++it)
        {
            std::cout << it->first << " " << it->second << "\n";
        }
        std::cout<<"DownLink"<<endl;
        for(auto it = downSize.cbegin(); it != downSize.cend(); ++it)
        {
            std::cout << it->first << " " << it->second << "\n";
        }*/

    }
    pcap_freealldevs(alldevs);
}
void Sniffer::print_hex_ascii_line(const char *payload, int len, int offset)
{

    int i;
    int gap;
    const char *ch;

    /* offset */
    printf("%05d   ", offset);

    /* hex */
    ch = payload;
    for(i = 0; i < len; i++) {
        printf("%02x ", *ch);
        ch++;
        /* print extra space after 8th byte for visual aid */
        if (i == 7)
            printf(" ");
    }
    /* print space to handle line less than 8 bytes */
    if (len < 8)
        printf(" ");

    /* fill hex gap with spaces if not full line */
    if (len < 16) {
        gap = 16 - len;
        for (i = 0; i < gap; i++) {
            printf("   ");
        }
    }
    printf("   ");

    /* ascii (if printable) */
    ch = payload;
    for(i = 0; i < len; i++) {
        if (isprint(*ch))
            printf("%c", *ch);
        else
            printf(".");
        ch++;
    }

    printf("\n");

    return;
}

/*
 * print packet payload data (avoid printing binary data)
 */
void Sniffer::print_payload(const char *payload, int len)
{

    int len_rem = len;
    int line_width = 16;			/* number of bytes per line */
    int line_len;
    int offset = 0;					/* zero-based offset counter */
    const char *ch = payload;

    if (len <= 0)
        return;

    /* data fits on one line */
    if (len <= line_width) {
        print_hex_ascii_line(ch, len, offset);
        return;
    }

    /* data spans multiple lines */
    for ( ;; ) {
        /* compute current line length */
        line_len = line_width % len_rem;
        /* print line */
        print_hex_ascii_line(ch, line_len, offset);
        /* compute total remaining */
        len_rem = len_rem - line_len;
        /* shift pointer to remaining bytes to print */
        ch = ch + line_len;
        /* add offset */
        offset = offset + line_width;
        /* check if we have line width chars or less */
        if (len_rem <= line_width) {
            /* print last line and get out */
            print_hex_ascii_line(ch, len_rem, offset);
            break;
        }
    }

    return;
}

/*
 * dissect/print packet
 */
void Sniffer::got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{

    static int count = 1;                   /* packet counter */

    /* declare pointers to packet headers */
    const struct sniff_ethernet *ethernet;  /* The ethernet header [1] */
    const struct sniff_ip *ip;              /* The IP header */
    const struct sniff_tcp *tcp;            /* The TCP header */

    const char *payload;                    /* Packet payload */
    pcap_dumper_t *pd;
    int size_ip;
    int size_tcp;
    int size_payload;

    printf("\nPacket number %d:\n", count);
    count++;

    /* define ethernet header */
    ethernet = (struct sniff_ethernet*)(packet);

    /* define/compute ip header offset */
    ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
    size_ip = IP_HL(ip)*4;
    if (size_ip < 20) {
        printf("   * Invalid IP header length: %u bytes\n", size_ip);
        return;
    }

    /* print source and destination IP addresses */
    printf("       From: %s\n", inet_ntoa(ip->ip_src));
    printf("         To: %s\n", inet_ntoa(ip->ip_dst));
    string src = string((const char*) inet_ntoa(ip->ip_src));
    string dst = string((const char*)inet_ntoa(ip->ip_dst));
    /* determine protocol */
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

    /*
     *  OK, this packet is TCP.
     */

    /* define/compute tcp header offset */
    tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
    size_tcp = TH_OFF(tcp)*4;
    /*if (size_tcp < 20) {
        printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
        return;
    }*/

    printf("   Src port: %d\n", ntohs(tcp->th_sport));
    printf("   Dst port: %d\n", ntohs(tcp->th_dport));

    /* define/compute tcp payload (segment) offset */
    payload = (const char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);

    /* compute tcp payload (segment) size */
    size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);
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
    std::cout<<timeArray<<endl;
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

    /*
     * Print payload data; it might be binary, so don't just
     * treat it as a string.
     */
    if (size_payload > 0) {
        printf("   Payload (%d bytes):\n", size_payload);
        print_payload(payload, size_payload);
    }



    ///////


    //pcap_dump_open_append()
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
