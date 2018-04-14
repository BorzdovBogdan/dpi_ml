LIB=-I/usr/include/ -I/usr/include/pcap 

all: main
main: Sniffer.o main_1.o
	gcc $(LIB) -o Sniffer.o main_1.o 
main_1.o: Sniffer.h Sniffer.cpp main_1.cpp
	gcc -std=c++11 -c Sniffer.cpp main_1.cpp