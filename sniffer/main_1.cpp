//
// Created by Bogdan on 09.04.18.
//

#include "Sniffer.h"

int main(int argc, char **argv){
    Sniffer *s = new Sniffer(argv[1][0]);
    s->networkSniffer();
    return 0;
}