LIB=-I/usr/include/ -I/usr/include/pcap 

sniff.elf: sniff.o Sniffer.o
	g++ -std=c++11 sniff.o Sniffer.o -lpcap -o sniff.elf
sniff.o: sniff.cpp Sniffer.h
	g++ -std=c++11 -c sniff.cpp -o sniff.o
Sniffer.o: Sniffer.cpp Sniffer.h
	g++ -std=c++11 -c Sniffer.cpp -o  Sniffer.o
clean:
	rm -f *.o *.elf