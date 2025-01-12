
# Bank Account Management System (UDP)

This is a simple bank account management system implemented using C, UDP sockets, and multithreading. The system allows multiple clients to connect to the server and perform operations such as adding funds to an account, withdrawing funds, checking the balance, and viewing the last 10 operations for an account.

## Features
- **Add Funds**: `AJOUT` command.
- **Withdraw Funds**: `RETRAIT` command.
- **Check Balance**: `SOLDE` command.
- **View Operations**: `OPERATIONS` command.
- **Lightweight Communication**: Uses UDP for fast, connectionless communication.

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Cloning the Repository](#cloning-the-repository)
3. [Compiling the Code](#compiling-the-code)
4. [Running the Server and Client](#running-the-server-and-client)
5. [Using the Commands](#using-the-commands)
6. [Adding New Accounts](#adding-new-accounts)
7. [Error Handling](#error-handling)
8. [Closing the Application](#closing-the-application)

---

## Prerequisites
Before you begin, ensure that you have the following installed:
- **C Compiler**: A C compiler like `gcc`.
- **Make**: A build automation tool for managing compilation.
- **Threads**: Ensure your system supports threading (usually supported by default on most systems).
- **Network Access**: Ensure that UDP port `8080` is available (or modify the port in the source code if necessary).

---

## Cloning the Repository

To clone the repository, use the following command in your terminal:

```bash
git clone <repository-url>
cd <repository-directory>
```

Replace `<repository-url>` with the actual URL of the repository.

---

## Compiling the Code

The project includes a Makefile to automate the compilation process.

1. Open a terminal in the project directory (where the Makefile is located).
2. To compile the server and client programs, run:

```bash
make
```

This will generate two executables:

- `server_udp` (Server)
- `client_udp` (Client)

The Makefile ensures that all necessary files (server_udp.c, client_udp.c, comptes.c) are compiled and linked properly.

---

## Running the Server and Client

### Running the Server

In your terminal, navigate to the directory containing the `server_udp` executable.

Start the server by running:

```bash
./server_udp
```

The server will start and listen for incoming UDP datagrams. You should see:

```bash
Serveur UDP en attente de connexions...
```

The server is now ready to process requests from clients.

### Running the Client

Open a new terminal window and navigate to the directory containing the `client_udp` executable.

Start the client by running:

```bash
./client_udp
```

The client will connect to the server and prompt you to enter a command.

---

## Using the Commands

Once the client is running, you can use the following commands to interact with the server:

### AJOUT (Add funds to an account)

**Usage**:

```bash
AJOUT <id_client> <id_compte> <password> <somme>
```

**Example**:

```bash
AJOUT 1 101 mypassword 500
```

- `id_client`: The ID of the client.
- `id_compte`: The account ID.
- `password`: The account password.
- `somme`: The amount to add to the account.

If the operation is successful, the server will respond:

```bash
OK - Montant 500.00 ajouté avec succès
```

On failure (e.g., invalid password), the response will be:

```bash
KO - Identifiant ou mot de passe incorrect
```

---

### RETRAIT (Withdraw funds from an account)

**Usage**:

```bash
RETRAIT <id_client> <id_compte> <password> <somme>
```

**Example**:

```bash
RETRAIT 1 101 mypassword 200
```

On success:

```bash
OK - Montant 200.00 retiré avec succès
```

On failure (e.g., insufficient funds):

```bash
KO - Solde insuffisant ou erreur
```

---

### SOLDE (Check the balance of an account)

**Usage**:

```bash
SOLDE <id_client> <id_compte> <password>
```

**Example**:

```bash
SOLDE 1 101 mypassword
```

Response:

```bash
OK - Solde du compte : 1000.00
```

On failure (e.g., account not found):

```bash
KO - Compte introuvable
```

---

### OPERATIONS (View the last 10 operations of an account)

**Usage**:

```bash
OPERATIONS <id_client> <id_compte> <password>
```

**Example**:

```bash
OPERATIONS 1 101 mypassword
```

Response:

```bash
OK - Historique des opérations:
AJOUT de 500.00 le 2025-01-12 12:30:00
RETRAIT de 200.00 le 2025-01-12 13:00:00
```

If no operations exist:

```bash
OK - Aucune opération trouvée
```

---

## Adding New Accounts

To add new accounts, modify the `init_clients()` function in `server_udp.c`.

1. Open `server_udp.c` and find the `init_clients()` function.
2. Follow the existing structure to add a new client.

**Example**:

```c
clients[3].id_client = 4;  // New client ID
clients[3].compte_count = 1;
clients[3].comptes[0].id_compte = 104;  // New account ID
strcpy(clients[3].comptes[0].password, "newpassword");
clients[3].comptes[0].solde = 200.0;
clients[3].comptes[0].op_count = 0;
```

3. Save the file and recompile the code using:

```bash
make
```

---

## Error Handling

- **Invalid Command Format**:

```bash
KO - Commande invalide
```

- **Wrong Parameters**:

```bash
KO - Identifiant ou mot de passe incorrect
```

- **Insufficient Funds**:

```bash
KO - Solde insuffisant ou erreur
```

---

## Closing the Application

- To stop the client, press `Ctrl+C` in the client terminal.
- To stop the server, press `Ctrl+C` in the server terminal.

---

## Conclusion

This Bank Account Management System provides a lightweight and fast implementation using UDP. It is ideal for applications that prioritize speed over guaranteed delivery. You can extend this project by adding more commands or integrating it into larger systems.
