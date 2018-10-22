//
// Created by c16fld on 2018-10-17.
//

#ifndef CHAT_SERVICE_CLIENT_LIST_H
#define CHAT_SERVICE_CLIENT_LIST_H

#include <malloc.h>
#include "../error_handling.h"

#define CLIENT_LIST_MAX_SIZE 256

typedef struct client {
    int socket;
    char *identity;
} client;

typedef struct client_list {
    client clients[256];
    int size;
    int num_connected_clients;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} client_list;

client_list *client_list_create();

int client_list_add_client(client c, client_list *cl);

int client_list_remove_client(client c, client_list *cl);

client client_list_get_client_from_index(int index, client_list *cl);

client client_list_get_client_from_identity(char *identity, client_list *cl);

client client_list_get_client_from_socket_id(int socket_id, client_list *cl);

int client_list_set_identity_to_socket(int socket, char *identity, client_list *cl, int identity_length);

int client_list_create_participants_string(client_list *cl, char** participants_string);
int client_list_get_num_connected_clients(client_list* cl);

void print_lock(client_list* cl);
void print_unlock(client_list* cl);
#endif //CHAT_SERVICE_CLIENT_LIST_H
