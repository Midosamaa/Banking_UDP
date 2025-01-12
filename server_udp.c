#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "comptes.h"

// Variables globales
#define MAX_CLIENTS 3
Client clients[MAX_CLIENTS];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex pour synchronisation des threads

// Fonction pour initialiser les clients et leurs comptes
void init_clients() {
    clients[0].id_client = 1;
    clients[0].compte_count = 1;
    clients[0].comptes[0].id_compte = 101;
    strcpy(clients[0].comptes[0].password, "mypassword");
    clients[0].comptes[0].solde = 1000.0;
    clients[0].comptes[0].op_count = 0;

    clients[1].id_client = 2;
    clients[1].compte_count = 1;
    clients[1].comptes[0].id_compte = 102;
    strcpy(clients[1].comptes[0].password, "pass123");
    clients[1].comptes[0].solde = 500.0;
    clients[1].comptes[0].op_count = 0;

    clients[2].id_client = 3;
    clients[2].compte_count = 1;
    clients[2].comptes[0].id_compte = 103;
    strcpy(clients[2].comptes[0].password, "securepass");
    clients[2].comptes[0].solde = 750.0;
    clients[2].comptes[0].op_count = 0;
}

// Fonction pour traiter la commande et générer la réponse
void traiter_commande(char *commande, char *reponse, int client_socket, struct sockaddr_in *client_addr, socklen_t *client_len) {
    int id_client, id_compte;
    char password[PASSWORD_LEN];
    float somme;

    if (sscanf(commande, "AJOUT %d %d %s %f", &id_client, &id_compte, password, &somme) == 4) {
        pthread_mutex_lock(&mutex);  // Verrouillage avant traitement
        if (verifier_identite(clients, MAX_CLIENTS, id_client, id_compte, password)) {
            if (ajouter_fonds(clients, id_client, id_compte, somme)) {
                sprintf(reponse, "OK - Montant %.2f ajouté avec succès", somme);
            } else {
                sprintf(reponse, "KO - Erreur lors de l'ajout");
            }
        } else {
            sprintf(reponse, "KO - Identifiant ou mot de passe incorrect");
        }
        pthread_mutex_unlock(&mutex);  // Déverrouillage après traitement
    }
    else if (sscanf(commande, "RETRAIT %d %d %s %f", &id_client, &id_compte, password, &somme) == 4) {
        pthread_mutex_lock(&mutex);
        if (verifier_identite(clients, MAX_CLIENTS, id_client, id_compte, password)) {
            if (retirer_fonds(clients, id_client, id_compte, somme)) {
                sprintf(reponse, "OK - Montant %.2f retiré avec succès", somme);
            } else {
                sprintf(reponse, "KO - Solde insuffisant ou erreur");
            }
        } else {
            sprintf(reponse, "KO - Identifiant ou mot de passe incorrect");
        }
        pthread_mutex_unlock(&mutex);
    }
    else if (sscanf(commande, "SOLDE %d %d %s", &id_client, &id_compte, password) == 3) {
        pthread_mutex_lock(&mutex);
        if (verifier_identite(clients, MAX_CLIENTS, id_client, id_compte, password)) {
            float solde = consulter_solde(clients, id_client, id_compte);
            if (solde >= 0) {
                sprintf(reponse, "OK - Solde du compte : %.2f", solde);
            } else {
                sprintf(reponse, "KO - Compte introuvable");
            }
        } else {
            sprintf(reponse, "KO - Identifiant ou mot de passe incorrect");
        }
        pthread_mutex_unlock(&mutex);
    }
    else if (sscanf(commande, "OPERATIONS %d %d %s", &id_client, &id_compte, password) == 3) {
        pthread_mutex_lock(&mutex);
        if (verifier_identite(clients, MAX_CLIENTS, id_client, id_compte, password)) {
            Operation operations[MAX_OPERATIONS];
            int op_count = consulter_operations(clients, id_client, id_compte, operations);
            if (op_count > 0) {
                strcpy(reponse, "OK - Historique des opérations:\n");
                for (int i = 0; i < op_count; i++) {
                    char operation[100];
                    struct tm *timeinfo = localtime(&operations[i].date);
                    char date_str[20];
                    strftime(date_str, sizeof(date_str), "%Y-%m-%d %H:%M:%S", timeinfo);
                    sprintf(operation, "%s de %.2f le %s\n", operations[i].type, operations[i].montant, date_str);
                    strcat(reponse, operation);
                }
            } else {
                sprintf(reponse, "OK - Aucune opération trouvée");
            }
        } else {
            sprintf(reponse, "KO - Identifiant ou mot de passe incorrect");
        }
        pthread_mutex_unlock(&mutex);
    } else {
        sprintf(reponse, "Commande non reconnue");
    }
}

int main() {
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Erreur création socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erreur bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Serveur UDP en attente de connexions...\n");

    init_clients();

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        char buffer[1024];
        char reponse[1024];

        int bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
        if (bytes_received <= 0) {
            perror("Erreur de réception");
            continue;
        }

        buffer[bytes_received] = '\0';  // Null terminate string
        printf("Reçu du client : %s\n", buffer);

        traiter_commande(buffer, reponse, server_socket, &client_addr, &client_len);

        if (sendto(server_socket, reponse, strlen(reponse), 0, (struct sockaddr *)&client_addr, client_len) == -1) {
            perror("Erreur d'envoi");
        }
    }

    close(server_socket);
    return 0;
}
