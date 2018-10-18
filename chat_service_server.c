//
// Created by c16fld on 2018-10-17.
//
#include <socket_handler/socket_helper.h>
#include <pthread.h>
#include <pdu_handler/pdu_helper.h>
#include <pdu_handler/server-nameserver/pdu_handler_server-nameserver.h>
#include <pdu_handler/client-server/pdu_handler_client-server.h>
#include "chat_service_server.h"
#include "socket_handler/socket_interface.h"



void server_run_server(int port){
    client_list* client_list = client_list_create();
    int server_socket = socket_tcp_server_create(port);

    start_accepter_thread(client_list, server_socket);
    start_heartbeat_thread(port, client_list);

    //alla sockets som inte har en identitet ska inv'nta pdu_join
    //Las meddelanden fran alla nadra och invanta pdu_quit. Vidarebefore alla meddelanden.
    //lamnar en klient, meddela alla.
    // joinar en klient, meddela alla.
    //

    server_message_forwarding(client_list);


    while(1);
}

void server_message_forwarding(client_list *client_list_arg) {
    int num_clients = client_list_arg->num_connected_clients;
    int connected_sockets[num_clients];
    int index = 0;
    for (int i = 0; i < CLIENT_LIST_MAX_SIZE; ++i) {
        client cl = client_list_arg->clients[i];
        if(cl.socket != 0){
            connected_sockets[index] = cl.socket;
            index++;
        }
    }

    PDU** responses = socket_read_pdu_from(connected_sockets, client_list_arg->num_connected_clients);

    for (int i = 0; i < num_clients; ++i) {
        if(responses[i] != NULL){
            switch (responses[i]->op){
                case OP_MESS: {
                    //broadcast message to clients
                    socket_write_pdu_to(responses[i], connected_sockets, num_clients);
                    free(responses[i]);
                    break;
                }
                case OP_QUIT: {
                    //notify everyone what client left
                    op_quit_response(client_list_arg, num_clients, connected_sockets, responses, i);
                    break;
                }
                case OP_JOIN: {
                    //notify everyone what client joined and send participants to newly connected client
                    op_join_response(client_list_arg, num_clients, connected_sockets, responses, i);
                    break;
                }
                default:
                    break;
            }
        }

    }
}

void op_quit_response(client_list *cl, int num_clients, int *connected_sockets, PDU **responses, int i) {
    client clie = client_list_get_client_from_socket_id(connected_sockets[i], cl);
    pdu_pleave* pleave = pdu_pleave_create(clie.identity);
    socket_write_pdu_to((PDU *) pleave, connected_sockets, num_clients);
    free(pleave);
    free(responses[i]);
}

void op_join_response(client_list *cl, int num_clients, int *connected_sockets, PDU **responses, int i) {
    pdu_join* join = (pdu_join *) responses[i];
    client_list_set_identity_to_socket(connected_sockets[i], (char *) join->identity, cl);
    char* participants_string;
    int num_partici = client_list_create_participants_string(cl, participants_string);
    pdu_participants* participants = pdu_participants_create(participants_string, num_partici);
    socket_write_pdu_to((PDU *) participants, &connected_sockets[i], 1);

    client clie = client_list_get_client_from_socket_id(connected_sockets[i], cl);
    pdu_pjoin* pjoin = pdu_pjoin_create(clie.identity);
    socket_write_pdu_to((PDU *) pjoin, connected_sockets, num_clients);

    free(participants_string);
    free(join);
    free(pjoin);
    free(participants);
}

static void start_accepter_thread(client_list *client_list, int server_socket) {
    pthread_t client_accepter_thread;

    server_accepting_arguments* acc_args = safe_calloc(1, sizeof(server_accepting_arguments));
    acc_args->cl = client_list;
    acc_args->server_socket = server_socket;
    pthread_create(&client_accepter_thread, NULL, &server_keep_accepting_clients, acc_args);
}

static void start_heartbeat_thread(int port, client_list *client_list) {
    server_heart_beat_arguments* hb_args = safe_calloc(1, sizeof(server_heart_beat_arguments));

    pthread_t heart_beat_thread;
    hb_args->cl = client_list;
    hb_args->own_address = calloc(1, 255);
    network_getFQDN(hb_args->own_address, 255);
    hb_args->own_port = port;
    hb_args->name_server_address = "itchy.cs.umu.se";
    hb_args->name_server_port = 1337;

    pthread_create(&heart_beat_thread, NULL, &server_start_heart_beat, hb_args);
    pthread_join(heart_beat_thread, 0);
}

static void* server_keep_accepting_clients(void* args){
    server_accepting_arguments* data = args;
    client_list* cl = data->cl;
    int server_socket = data->server_socket;
    while(server_socket != -1){
        while(cl->num_connected_clients < CLIENT_LIST_MAX_SIZE){
            client clnt;
            clnt.socket = socket_tcp_get_connecting_socket(server_socket);
            clnt.identity = NULL;
            client_list_add_client(clnt, cl);
        }
        sleep(2);
    }
    free(data);
}

void *server_read_and_forward_messages(void* args){

}

static void *server_start_heart_beat(void *args){
    server_heart_beat_arguments* heartbeat_args = args;
    int name_server_socket = socket_udp_name_server_socket(
            heartbeat_args->name_server_port, heartbeat_args->name_server_address);

    reg* reg = pdu_create_reg((int) strlen(heartbeat_args->own_address), heartbeat_args->own_port, heartbeat_args->own_address);


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(true) {
        socket_write_pdu_to((PDU *) reg, &name_server_socket, 1);
        ack *pdu_ack = NULL;

        //keep acking until nameserver accepts.
        while (pdu_ack == NULL) {
            pdu_ack = socket_read_ack_from_udp(name_server_socket);
            if (pdu_ack == NULL) {
                socket_write_pdu_to((PDU *) reg, &name_server_socket, 1);
            }
        }
        uint16_t id = pdu_ack->id_number;

        not_reg *not_reg = NULL;
        while (not_reg == NULL) {
            alive *alive = pdu_create_alive(heartbeat_args->cl->num_connected_clients, id);
            socket_write_pdu_to((PDU *) alive, &name_server_socket, 1);
            not_reg = socket_read_not_reg_from_udp(name_server_socket);
            sleep(2);

            //if pdu is ack instead of not_reg, ignore it.
            if (not_reg != NULL) {
                if (not_reg->pdu.op != 100) {
                    free(not_reg);
                    not_reg = NULL;
                }
            }
        }
    }
#pragma clang diagnostic pop

}
