
all: server client

server: server.cpp libTCP.h libTCP.cpp
	g++ -std=c++11 server.cpp libTCP.cpp -o server

client: client.cpp libTCP.h libTCP.cpp
	g++ -std=c++11 client.cpp libTCP.cpp -o client

clean:
	rm -f server client
