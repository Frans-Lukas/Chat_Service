//
// Created by c16fld on 2018-10-17.
//
#include <socket_handler/socket_helper.h>
#include <pthread.h>
#include <pdu_handler/pdu_helper.h>
#include <pdu_handler/server-nameserver/pdu_handler_server-nameserver.h>
#include "chat_service_server.h"
#include "socket_handler/socket_interface.h"

void server_run_server(int port){
    client_list* client_list = client_list_create();
    int server_socket = socket_tcp_server_create(port);

    //keep accepting clients until max client limit
    pthread_t client_accepter_thread;
    pthread_t heart_beat_thread;

    server_accepting_arguments* acc_args = safe_calloc(1, sizeof(server_accepting_arguments));
    acc_args->cl = client_list;
    acc_args->server_socket = server_socket;
    pthread_create(&client_accepter_thread, NULL, &server_keep_accepting_clients, acc_args);

    server_heart_beat_arguments* hb_args = safe_calloc(1, sizeof(server_heart_beat_arguments));

    hb_args->cl = client_list;
    hb_args->own_address = calloc(1, 255);
    network_getFQDN(hb_args->own_address, 255);
    hb_args->own_port = port;
    hb_args->name_server_address = "itchy.cs.umu.se";
    hb_args->name_server_port = 1337;

    pthread_create(&heart_beat_thread, NULL, &server_start_heart_beat, hb_args);
    pthread_join(heart_beat_thread, 0);
    while(1);
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
    free(data);
}

void *server_read_and_forward_messages(void* args){

}

void *server_start_heart_beat(void *args){
    server_heart_beat_arguments* heartbeat_args = args;
    int name_server_socket = socket_udp_name_server_socket(
            heartbeat_args->name_server_port, heartbeat_args->name_server_address);

    reg* reg = pdu_create_reg((int) strlen(heartbeat_args->own_address), heartbeat_args->own_port, heartbeat_args->own_address);

    socket_write_pdu_to((PDU*)reg, &name_server_socket, 1);
    ack* pdu_ack = NULL;

    while(pdu_ack == NULL){
        fprintf(stderr, "PDU_ACK == NULL\n");
        pdu_ack = socket_read_ack_from_udp(name_server_socket);
        if(pdu_ack == NULL){
            socket_write_pdu_to((PDU*)reg, &name_server_socket, 1);
        }
    }
    uint16_t id = pdu_ack->id_number;

    not_reg* not_reg = NULL;
    while(not_reg == NULL) {

     fprintf(stderr, "PDU_REG == NULL\n");

        alive* alive = pdu_create_alive(heartbeat_args->cl->num_connected_clients, id);
        socket_write_pdu_to((PDU *) alive, &name_server_socket, 1);
        not_reg = socket_read_not_reg_from_udp(name_server_socket);
    }

}
