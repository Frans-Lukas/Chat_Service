//
// Created by c16fld on 2018-10-17.
//

#ifndef CHAT_SERVICE_CLIENT_LIST_H
#define CHAT_SERVICE_CLIENT_LIST_H

#include <malloc.h>
#include "../error_handling.h"

#define CLIENT_LIST_MAX_SIZE 256

typedef struct client_list{
    int clients[256];
} client_list;

client_list* client_list_create();
int client_list_add_client(int socket, client_list* cl);
int client_list_remove_client(int socket, client_list* cl);
int client_list_get_client(int index, client_list* cl);

#endif //CHAT_SERVICE_CLIENT_LIST_H
