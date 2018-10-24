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
    char* own_server_name;
    client_list* cl;
} server_heart_beat_arguments;

static void* server_keep_accepting_clients(void* args);
void server_run_server(int port, char* server_name, char* name_server_adress, int name_server_port);
static void *server_start_heart_beat(void *args);
ack* socket_read_ack_from_udp(int socket);
static void start_accepter_thread(client_list *client_list, int server_socket);
static void start_heartbeat_thread(int port, client_list *client_list, char* server_name ,char* name_server, int name_server_port);
void server_message_forwarding(client_list *client_list_arg);
void op_join_response(client_list *cl, int num_clients, int* connected_sockets, pdu_join* pdu, int index);
void op_quit_response(client_list *cl, int num_clients, int* connected_socket, pdu_quit *pdu, int i);
void op_mess_response(int num_clients, int *connected_sockets, PDU* response, int i, client_list* cl);
void send_participants_list_to_socket(client_list *cl, int socket);
void send_pjoin_to_sockets(client_list *cl, int num_clients, int *connected_sockets, int index);
void free_response(PDU *responses);


#endif //CHAT_SERVICE_CHAT_SERVICE_SERVER_H
