# Compiler and flags
CC = gcc
CFLAGS = -Wall -pthread

# Source files
SERVER_SRC = server_udp.c
CLIENT_SRC = client_udp.c
COMPTES_SRC = comptes.c

# Object files
SERVER_OBJ = server_udp.o
CLIENT_OBJ = client_udp.o
COMPTES_OBJ = comptes.o

# Executables
SERVER_EXEC = server_udp
CLIENT_EXEC = client_udp

# Rule for compiling the server object file
server_udp.o: $(SERVER_SRC) comptes.h
	$(CC) $(CFLAGS) -c $(SERVER_SRC) -o server_udp.o

# Rule for compiling the client object file
client_udp.o: $(CLIENT_SRC) comptes.h
	$(CC) $(CFLAGS) -c $(CLIENT_SRC) -o client_udp.o

# Rule for compiling the comptes object file
comptes.o: comptes.c comptes.h
	$(CC) $(CFLAGS) -c comptes.c -o comptes.o

# Rule to build the server executable
$(SERVER_EXEC): server_udp.o comptes.o
	$(CC) $(CFLAGS) -o $(SERVER_EXEC) server_udp.o comptes.o

# Rule to build the client executable
$(CLIENT_EXEC): client_udp.o comptes.o
	$(CC) $(CFLAGS) -o $(CLIENT_EXEC) client_udp.o comptes.o

# Clean up object files and executables
clean:
	rm -rf  $(SERVER_EXEC) $(CLIENT_EXEC) $(SERVER_OBJ) $(CLIENT_OBJ) $(COMPTES_OBJ)

# Rebuild everything
rebuild: clean all

# Build both server and client
all: $(SERVER_EXEC) $(CLIENT_EXEC)
