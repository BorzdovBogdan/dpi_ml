//
// Created by Bogdan on 09.04.18.
//

#include "Sniffer.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <tclap/CmdLine.h>
using namespace std;
int main(int argc, char **argv){
	
   	Sniffer *s = new Sniffer(40);
	char mode = argv[1][0];
    if(mode == 'n'){
    	cout<<"network sniffering"<<endl;
        s->sniffNetwork();
    }else if(mode == 'w'){
    	cout<<"file sniffering"<<endl;
        s->sniffNetworkAndWrite();
    }
    return 0;
}