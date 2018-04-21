libPrefix=-L/home/bogdan/tensorflow/tensorflow/contrib/makefile/gen
INC=-I/home/bogdan/tensorflow -I/home/bogdan/tensorflow/tensorflow/contrib/makefile/downloads/eigen/ -I/home/bogdan/tensorflow/tensorflow/contrib/makefile/gen/host_obj -I/home/bogdan/tensorflow/tensorflow/contrib/makefile/gen/protobuf/include
LIB=$(libPrefix)/lib -L/usr/lib
main.elf: main.o
	g++ -std=c++11 main.o $(LIB) -ltensorflow_framework -ltensorflow-core -o main_1.elf
main.o: main.cpp
	g++ -std=c++11 $(INC) -c main.cpp -o main.o