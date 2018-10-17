//
// Created by c16fld on 2018-10-17.
//

#ifndef CHAT_SERVICE_CHAT_SERVICE_SERVER_H
#define CHAT_SERVICE_CHAT_SERVICE_SERVER_H

#include "error_handling.h"
#include <socket_handler/client_list.h>

typedef struct server_accepting_arguments{
    int server_socket;
    client_list* cl;
} server_accepting_arguments;

void server_keep_accepting_clients(void* args);
void server_run_server();

#endif //CHAT_SERVICE_CHAT_SERVICE_SERVER_H
