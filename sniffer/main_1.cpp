//
// Created by Bogdan on 09.04.18.
//

#include "Sniffer.h"

int main(int argc, char **argv){
    Sniffer *s = new Sniffer();
    char mode = argv[1][0];
    if(mode == 'n'){
        s->sniffNetwork();
    }else if(mode == 'w'){
        s->sniffNetworkAndWrite();
    }
    return 0;
}