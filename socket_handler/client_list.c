//
// Created by c16fld on 2018-10-17.
//

#include <errno.h>
#include "client_list.h"

client_list *client_list_create() {
    client_list* cl = safe_calloc(1, sizeof(client_list));
    cl->size = CLIENT_LIST_MAX_SIZE;
    cl->num_connected_clients = 0;
    return cl;
}

int client_list_add_client(int socket, client_list *cl) {
    for (int i = cl->num_connected_clients; i < CLIENT_LIST_MAX_SIZE; ++i) {
        if(cl->clients[i] == 0){
            cl->clients[i] = socket;
            cl->num_connected_clients++;
            return 0;
        }
    }
    perror("Client list full, can't add more clients.");
    return -1;
}

int client_list_remove_client(int socket, client_list *cl) {
    for (int i = 0; i < cl->num_connected_clients; ++i) {
        if(cl->clients[i] == socket){
            cl->clients[i] = 0;
            cl->num_connected_clients--;
            return 0;
        }
    }
    perror("No client with socket value found in client list.");
    return -1;
}

int client_list_get_client(int index, client_list *cl) {
    if(index >= CLIENT_LIST_MAX_SIZE){
        perror("Index out of bounds on client list");
        return -1;
    }
    return cl->clients[index];
}