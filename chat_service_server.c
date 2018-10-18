//
// Created by c16fld on 2018-10-17.
//
#include <socket_handler/socket_helper.h>
#include <pthread.h>
#include <pdu_handler/pdu_helper.h>
#include "chat_service_server.h"
#include "socket_handler/socket_interface.h

void server_run_server(int port){
    client_list* client_list = client_list_create();
    int server_socket = socket_tcp_server_create(port);

    //keep accepting clients until max client limit
    pthread_t client_accepter;
    server_accepting_arguments* args = safe_calloc(1, sizeof(server_accepting_arguments));

    pthread_create(&client_accepter, NULL, &server_keep_accepting_clients, args);
}

void* server_keep_accepting_clients(void* args){
    server_accepting_arguments* data = args;
    client_list* cl = data->cl;
    int server_socket = data->server_socket;
    while(server_socket != -1){
        while(cl->num_connected_clients < CLIENT_LIST_MAX_SIZE){
            client_list_add_client(socket_tcp_get_connecting_socket(server_socket), cl);
        }
        sleep(2);
    }
}

void *server_read_and_forward_messages(void* args){

}

void *server_start_heart_beat(void *args){
    server_heart_beat_arguments* heartbeat_args = args;
    int name_server_socket = socket_udp_name_server_socket(
            heartbeat_args->name_server_port, heartbeat_args->name_server_address);
    while(1){
        PDU*
    }

}
