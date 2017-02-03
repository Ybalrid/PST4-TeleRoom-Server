CC=g++ --std=c++14
LDFLAG= -lRakNetDLL
IFLAG= -I/usr/include/raknet -I./external/PST4-Teleroom-common

all: PST4Server

PST4NetServer.o: PST4NetServer.cpp PST4NetServer.hpp external/PST4-Teleroom-common/PST4Packets.hpp
	$(CC) -c PST4NetServer.cpp -o PST4NetServer.o $(LDFLAG) $(IFLAG)

main.o: main.cpp
	$(CC) -c main.cpp -o main.o $(LDFLAG) $(IFLAG)

PST4Server: PST4NetServer.o main.o
	$(CC) main.o PST4NetServer.o $(LDFLAG) $(IFLAG) -o PST4Server
