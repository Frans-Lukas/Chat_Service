//
// Created by c16fld on 2018-10-17.
//

#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <pdu_handler/pdu_helper.h>
#include "client_list.h"

client_list *client_list_create() {
    client_list *cl = safe_calloc(1, sizeof(client_list));
    cl->size = CLIENT_LIST_MAX_SIZE;
    cl->num_connected_clients = 0;
    cl->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    cl->cond = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    return cl;
}

void print_lock(client_list* cl){
    pthread_mutex_lock(&cl->mutex);
}
void print_unlock(client_list* cl){
    pthread_mutex_unlock(&cl->mutex);

}

int client_list_add_client(client c, client_list *cl) {
    print_lock(cl);
    for (int i = 0; i < CLIENT_LIST_MAX_SIZE; ++i) {
        if (cl->clients[i].socket == 0) {
            cl->clients[i] = c;
            cl->num_connected_clients++;
            pthread_mutex_unlock(&cl->mutex);
            return 0;
        }
    }
    perror("Client list full, can't add more clients.");
    pthread_mutex_unlock(&cl->mutex);
    return -1;
}

int client_list_remove_client(client c, client_list *cl) {
    print_lock(cl);
    for (int i = 0; i < CLIENT_LIST_MAX_SIZE; ++i) {
        if (cl->clients[i].socket == c.socket) {
            cl->clients[i].socket = 0;
            cl->clients[i].identity = NULL;
            cl->num_connected_clients--;
            pthread_mutex_unlock(&cl->mutex);
            return 0;
        }
    }
    perror("No client with socket value found in client list.");
    pthread_mutex_unlock(&cl->mutex);
    return -1;
}

client client_list_get_client_from_index(int index, client_list *cl) {
    print_lock(cl);
    if (index >= CLIENT_LIST_MAX_SIZE) {
        pthread_mutex_unlock(&cl->mutex);
        perror_exit("Index out of bounds on client list");
    }
    pthread_mutex_unlock(&cl->mutex);
    return cl->clients[index];
}

client client_list_get_client_from_socket_id(int socket_id, client_list *cl) {
    print_lock(cl);
    for (int i = 0; i < CLIENT_LIST_MAX_SIZE; ++i) {
        if (cl->clients[i].socket == socket_id) {
            pthread_mutex_unlock(&cl->mutex);
            return cl->clients[i];
        }
    }
    pthread_mutex_unlock(&cl->mutex);
    client empty_client;
    perror("Could not find client from socket id");
    return empty_client;
}

int client_list_set_identity_to_socket(int socket, char *identity, client_list *cl) {
    print_lock(cl);
    for (int i = 0; i < CLIENT_LIST_MAX_SIZE; ++i) {
        if (cl->clients[i].socket == socket) {
            cl->clients[i].identity = identity;
            pthread_mutex_unlock(&cl->mutex);
            return 0;
        }
    }
    pthread_mutex_unlock(&cl->mutex);
    perror("Could not find client to attach identity to.");
    return -1;
}


client client_list_get_client_from_identity(char *identity, client_list *cl) {
    print_lock(cl);
    for (int i = 0; i < CLIENT_LIST_MAX_SIZE; ++i) {
        if (strcmp(cl->clients[i].identity, identity) == 0) {
            pthread_mutex_unlock(&cl->mutex);
            return cl->clients[i];
        }
    }
    pthread_mutex_unlock(&cl->mutex);
    client empty_client;
    perror("Could not find client from identity");
    return empty_client;
}

int client_list_create_participants_string(client_list *cl, char **participants_string) {
    int num_identified_sockets = 0;

    print_lock(cl);
    for (int i = 0; i < CLIENT_LIST_MAX_SIZE; ++i) {
        if (cl->clients[i].identity != NULL) {
            num_identified_sockets++;
        }
    }

    char *client_array[num_identified_sockets];

    for (int i = 0; i < CLIENT_LIST_MAX_SIZE; ++i) {
        if (cl->clients[i].identity != NULL) {
            client_array[i] = cl->clients[i].identity;
        }
    }
    *participants_string = array_to_string(client_array, num_identified_sockets);
    pthread_mutex_unlock(&cl->mutex);

    return num_identified_sockets;
}