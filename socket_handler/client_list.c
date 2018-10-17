//
// Created by c16fld on 2018-10-17.
//

#include <errno.h>
#include <pthread.h>
#include "client_list.h"

client_list *client_list_create() {
    client_list* cl = safe_calloc(1, sizeof(client_list));
    cl->size = CLIENT_LIST_MAX_SIZE;
    cl->num_connected_clients = 0;
    cl->mutex = PTHREAD_MUTEX_INITIALIZER;
    cl->cond = PTHREAD_COND_INITIALIZER;
    return cl;
}

int client_list_add_client(int socket, client_list *cl) {
    pthread_mutex_lock(&cl->mutex);
    for (int i = cl->num_connected_clients; i < CLIENT_LIST_MAX_SIZE; ++i) {
        if(cl->clients[i] == 0){
            cl->clients[i] = socket;
            cl->num_connected_clients++;
            pthread_mutex_unlock(&cl->mutex);
            return 0;
        }
    }
    perror("Client list full, can't add more clients.");
    pthread_mutex_unlock(&cl->mutex);
    return -1;
}

int client_list_remove_client(int socket, client_list *cl) {
    pthread_mutex_lock(&cl->mutex);
    for (int i = 0; i < cl->num_connected_clients; ++i) {
        if(cl->clients[i] == socket){
            cl->clients[i] = 0;
            cl->num_connected_clients--;
            pthread_mutex_unlock(&cl->mutex);
            return 0;
        }
    }
    perror("No client with socket value found in client list.");
    pthread_mutex_unlock(&cl->mutex);
    return -1;
}

int client_list_get_client(int index, client_list *cl) {
    pthread_mutex_lock(&cl->mutex);
    if(index >= CLIENT_LIST_MAX_SIZE){
        perror("Index out of bounds on client list");
        pthread_mutex_unlock(&cl->mutex);
        return -1;
    }
    pthread_mutex_unlock(&cl->mutex);
    return cl->clients[index];
}