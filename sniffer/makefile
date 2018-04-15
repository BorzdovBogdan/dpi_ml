LIB=-I/usr/include/ -I/usr/include/pcap 

main.elf: main_1.o Sniffer.o
	gcc -std=c++11 main_1.o Sniffer.o -o main_1.elf
main_1.o: main_1.cpp Sniffer.h
	gcc -std=c++11 -c main_1.cpp -o main_1.o
Sniffer.o: Sniffer.cpp Sniffer.h
	gcc -std=c++11 $(LIB) -c Sniffer.cpp -o Sniffer.o