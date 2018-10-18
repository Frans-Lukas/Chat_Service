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

typedef struct server_heart_beat_arguments{
    int name_server_port;
    char* name_server_address;
    int own_port;
    char* own_address;
    client_list* cl;
} server_heart_beat_arguments;

void* server_keep_accepting_clients(void* args);
void server_run_server(int port);
void *server_start_heart_beat(void *args);
ack* socket_read_ack_from_udp(int socket);

#endif //CHAT_SERVICE_CHAT_SERVICE_SERVER_H
