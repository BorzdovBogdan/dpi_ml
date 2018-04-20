libPrefix=-L/home/bogdan/tensorflow/tensorflow/contrib/makefile/gen
INC=-I/home/bogdan/.local/lib/python3.5/site-packages/tensorflow/include/ -I/home/bogdan/.local/lib/python3.5/site-packages/tensorflow/include/external/nsync/public/
LIB=$(libPrefix)/protobuf/ $(libPrefix)/lib -L/usr/lib
main.elf: main.o
	g++ -std=c++11 main.o $(LIB) -ltensorflow_framework -ltensorflow-core -lprotobuf -o main_1.a
main.o: main_dlib.cpp
	g++ -std=c++11 -c main.cpp -o main.o