LIB=-I/usr/include/ -I/usr/include/pcap 

main.elf: main_1.o Sniffer.o
	g++ -std=c++11 main_1.o Sniffer.o -lpcap -o main_1.elf
main_1.o: main_1.cpp Sniffer.h
	g++ -std=c++11 -c main_1.cpp -o main_1.o
Sniffer.o: Sniffer.cpp Sniffer.h
	g++ -std=c++11 -c Sniffer.cpp -o  Sniffer.o