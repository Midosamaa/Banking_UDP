CC = gcc
CFLAGS = -Wall -pthread
SRC = server_udp.c client_udp.c comptes.c
OBJ = $(SRC:.c=.o)
EXEC = server_udp client_udp

all: $(EXEC)

server_udp: server_udp.o comptes.o
	$(CC) $(CFLAGS) -o server_udp server_udp.o comptes.o

client_udp: client_udp.o comptes.o
	$(CC) $(CFLAGS) -o client_udp client_udp.o comptes.o

clean:
	rm -f $(OBJ) $(EXEC)
