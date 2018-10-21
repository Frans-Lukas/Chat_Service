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

static void* server_keep_accepting_clients(void* args);
void server_run_server(int port);
static void *server_start_heart_beat(void *args);
static ack* socket_read_ack_from_udp(int socket);
static void start_accepter_thread(client_list *client_list, int server_socket);
static void start_heartbeat_thread(int port, client_list *client_list);
void server_message_forwarding(client_list *client_list_arg);
void op_join_response(client_list *cl, int num_clients, int connected_socket, pdu_join* pdu);
void op_quit_response(client_list *cl, int num_clients, int connected_socket, pdu_quit* pdu);


#endif //CHAT_SERVICE_CHAT_SERVICE_SERVER_H
