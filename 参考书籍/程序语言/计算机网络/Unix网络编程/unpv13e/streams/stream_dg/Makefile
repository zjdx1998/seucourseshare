all: client server

client: client.o net_stream.o
	cc client.o net_stream.o -o client -lnsl

server: server.o net_stream.o
	cc server.o net_stream.o -o server -lnsl
