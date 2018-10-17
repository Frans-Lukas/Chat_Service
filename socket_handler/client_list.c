//
// Created by c16fld on 2018-10-17.
//

#include <errno.h>
#include "client_list.h"

client_list *client_list_create() {
    client_list* cl = safe_calloc(1, sizeof(client_list));
    return cl;
}

int client_list_add_client(int socket, client_list *cl) {
    int isFull = 0;
    for (int i = 0; i < CLIENT_LIST_MAX_SIZE; ++i) {
        if(cl->clients[i] == 0){
            cl->clients[i] = socket;
            return 0;
        }
    }
    return -1;
}

int client_list_remove_client(int socket, client_list *cl) {
    return NULL;
}

int client_list_get_client(int index, client_list *cl) {
    return 0;
}
