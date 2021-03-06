//
// Created by c16fld on 2018-10-17.
//
#define _GNU_SOURCE


#include <socket_handler/socket_helper.h>
#include <pthread.h>
#include <pdu_handler/pdu_helper.h>
#include <pdu_handler/server-nameserver/pdu_handler_server-nameserver.h>
#include <pdu_handler/client-server/pdu_handler_client-server.h>
#include <assert.h>
#include "chat_service_server.h"
#include "server-nameserver/pdu_handler_server-nameserver.h"
#include "socket_handler/socket_interface.h"

void server_run_server(int port, char *server_name, char *name_server_adress, int name_server_port) {
    client_list *client_list = client_list_create();
    int server_socket = socket_tcp_server_create(port);

    start_accepter_thread(client_list, server_socket);
    start_heartbeat_thread(port, client_list, server_name, name_server_adress, name_server_port);

    while (1) {
        server_message_forwarding(client_list);
    }
}

void server_message_forwarding(client_list *clint_list) {
    int num_clients = client_list_get_num_connected_clients(clint_list);
    int connected_sockets[num_clients];
    int index = 0;
    int size = clint_list->num_connected_clients;
    for (int i = 0; i < CLIENT_LIST_MAX_SIZE; ++i) {
        client cl = clint_list->clients[i];
        if (size == index) {
            break;
        }
        if (cl.socket != 0) {
            connected_sockets[index] = cl.socket;
            index++;
        }
    }
    PDU **responses = socket_read_pdu_from(connected_sockets, client_list_get_num_connected_clients(clint_list),
                                           clint_list);
    if (responses != NULL) {
        for (int i = 0; i < index; ++i) {
            if (responses[i] != NULL) {
                if (responses[i]->op != 0) {
                    switch (responses[i]->op) {
                        case OP_MESS: {
                            //broadcast message to clients
                            fprintf(stderr, "Recieved MESS\n");
                            if (create_checksum((pdu_mess *) responses[i]) == 255) {
                                op_mess_response(num_clients, connected_sockets, responses[i], i, clint_list);
                            }
                            break;
                        }
                        case OP_QUIT: {
                            fprintf(stderr, "Recieved QUIT\n");
                            //notify everyone what client left
                            op_quit_response(clint_list, num_clients, connected_sockets, (pdu_quit *) responses[i], i);
                            break;
                        }
                        case OP_JOIN: {
                            fprintf(stderr, "Recieved JOIN\n");
                            //notify everyone what client joined and send participants to newly connected client
                            op_join_response(clint_list, num_clients, connected_sockets, (pdu_join *) responses[i], i);
                            break;
                        }
                        default:
                            break;
                    }
                    free_response(responses[i]);
                } else {
                    free(responses[i]);
                }
            }
        }
    }
    free(responses);
}

void free_response(PDU *responses) {
    switch (responses->op) {
        case OP_MESS: {
            //broadcast message to clients
            pdu_mess *pdu = (pdu_mess *) responses;
            free(pdu->client_identity);
            free(pdu->message);
            free(pdu);
            break;
        }
        case OP_QUIT: {
            //notify everyone what client left
            pdu_quit *pdu = (pdu_quit *) responses;
            free(pdu);
            break;
        }
        case OP_JOIN: {
            pdu_join *pdu = (pdu_join *) responses;
            free(pdu->identity);
            free(pdu);
            //notify everyone what client joined and send participants to newly connected client
            break;
        }
        case OP_REG: {
            reg *pdu = (reg *) responses;
            free(pdu->server_name);
            free(pdu);
        }
        case OP_PJOIN: {
            pdu_pjoin *pdu = (pdu_pjoin *) responses;
            free(pdu->client_identity);
            free(pdu);
        }
        case OP_PLEAVE: {
            pdu_pleave *pdu = (pdu_pleave *) responses;
            free(pdu->client_identity);
            free(pdu);
        }
        case OP_PARTICIPANTS: {
            pdu_participants *pdu = (pdu_participants *) responses;
            free(pdu->participant_names);
            free(pdu);
        }
        default:
            free(responses);
            break;
    }
}

void op_mess_response(int num_clients, int *connected_sockets, PDU *response, int i, client_list *cl) {

    pdu_mess *mess = (pdu_mess *) response;
    mess->timestamp = (uint32_t) time(NULL);
    client clie = client_list_get_client_from_socket_id(connected_sockets[i], cl);
    if (clie.identity != NULL) {
        mess->identity_length = (uint8_t) strlen(clie.identity);
        mess->client_identity = calloc(sizeof(uint32_t), (size_t) get_num_words(mess->identity_length, 4));
        pdu_cpy_chars(mess->client_identity, clie.identity, 0, mess->identity_length);
    } else {
        char *unkown_identity = "Unkown Identity\0";
        mess->identity_length = (uint8_t) strlen(unkown_identity);
        mess->client_identity = calloc(sizeof(uint32_t), (size_t) get_num_words(mess->identity_length, 4));
        pdu_cpy_chars(mess->client_identity, unkown_identity, 0, mess->identity_length);
    }
    mess->checksum = 0;
    mess->checksum = create_checksum(mess);

    if (socket_write_pdu_to((PDU *) mess, connected_sockets, num_clients) == -1) {
        perror("Failed to write to MESS sockets\n");
    }
}

void op_quit_response(client_list *cl, int num_clients, int *connected_socket, pdu_quit *pdu, int i) {
    client clie = client_list_get_client_from_socket_id(connected_socket[i], cl);
    pdu_pleave *pleave = pdu_pleave_create(clie.identity);
    socket_write_pdu_to((PDU *) pleave, connected_socket, num_clients);
    free(pleave->client_identity);
    free(pleave);
}

void op_join_response(client_list *cl, int num_clients, int *connected_sockets, pdu_join *pdu, int index) {

    if(pdu->identity_length == 0 || pdu->identity == NULL){
        pdu_mess* invalid_name_message = pdu_mess_create("", "Don't join with invalid name!");
        invalid_name_message->timestamp = (uint32_t) time(NULL);
        invalid_name_message->checksum = 0;
        invalid_name_message->checksum = create_checksum(invalid_name_message);
        socket_write_pdu_to((PDU *) invalid_name_message, connected_sockets, num_clients);
        disconnect_client_from_client_list(cl, connected_sockets[index]);
        free(invalid_name_message->message);
        free(invalid_name_message);
        return;
    }

    client_list_set_identity_to_socket(connected_sockets[index], (char *) pdu->identity, cl, pdu->identity_length);
    send_participants_list_to_socket(cl, connected_sockets[index]);
    send_pjoin_to_sockets(cl, num_clients, connected_sockets, index);

    pdu_mess* welcome_message = pdu_mess_create("", "\033[31;42mWelcome!.\033[0m");
    welcome_message->timestamp = (uint32_t) time(NULL);
    welcome_message->checksum = 0;
    welcome_message->checksum = create_checksum(welcome_message);
    socket_write_pdu_to((PDU *) welcome_message, connected_sockets, num_clients);
    free(welcome_message->client_identity);
    free(welcome_message->message);
    free(welcome_message);
}

void send_pjoin_to_sockets(client_list *cl, int num_clients, int *connected_sockets, int index) {
    client clie = client_list_get_client_from_socket_id(connected_sockets[index], cl);
    pdu_pjoin *pjoin;
    if (clie.identity != NULL) {
        pjoin = pdu_pjoin_create(clie.identity);
    } else {
        perror("Unkwon identity tried sending Pjoin.");
        pjoin = pdu_pjoin_create("Unkown identity");
    }
    fprintf(stderr, "Client %s joined the server.\n", clie.identity);
    socket_write_pdu_to((PDU *) pjoin, connected_sockets, num_clients);

    if (pjoin->client_identity != NULL) {
        free(pjoin->client_identity);
    }
    free(pjoin);
}

void send_participants_list_to_socket(client_list *cl, int socket) {
    char *participants_string;
    int *length = calloc(1, sizeof(int));
    int num_partici = client_list_create_participants_string(cl, &participants_string, length);
    pdu_participants *participants = pdu_participants_create(participants_string, num_partici, *length);
    free(length);
    socket_write_pdu_to((PDU *) participants, &socket, 1);
    if (participants->participant_names != NULL) {
        free(participants->participant_names);
    }
    free(participants);
    free(participants_string);
}

static void start_accepter_thread(client_list *client_list, int server_socket) {
    pthread_t client_accepter_thread;
    server_accepting_arguments *acc_args = safe_calloc(1, sizeof(server_accepting_arguments));
    acc_args->cl = client_list;
    acc_args->server_socket = server_socket;
    pthread_create(&client_accepter_thread, NULL, &server_keep_accepting_clients, acc_args);
}

static void
start_heartbeat_thread(int port, client_list *client_list, char *server_name, char *name_server, int name_server_port) {
    server_heart_beat_arguments *hb_args = safe_calloc(1, sizeof(server_heart_beat_arguments));
    pthread_t heart_beat_thread;
    hb_args->cl = client_list;
    hb_args->own_address = calloc(1, 255);
    network_getFQDN(hb_args->own_address, 255);
    hb_args->own_port = port;
    hb_args->name_server_address = name_server;
    hb_args->name_server_port = name_server_port;
    hb_args->own_server_name = server_name;


    pthread_create(&heart_beat_thread, NULL, &server_start_heart_beat, hb_args);
}


static void *server_keep_accepting_clients(void *args) {
    server_accepting_arguments *data = args;
    client_list *cl = data->cl;
    int server_socket = data->server_socket;
    while (1) {
        while (client_list_get_num_connected_clients(cl) < CLIENT_LIST_MAX_SIZE) {
            client clnt;
            clnt.socket = socket_tcp_get_connecting_socket_by_accepting(server_socket);
            clnt.identity = NULL;
            if(clnt.socket == -1){
                continue;
            }
            if (client_list_get_num_connected_clients(cl) >= 255) {
                close(clnt.socket);
                fprintf(stderr, "Server full, disconnecting newly connected socket.\n");
            } else {
                fprintf(stderr, "New client joined.\n");
                client_list_add_client(clnt, cl);
            }
        }
        sleep(2);
    }
}


static void *server_start_heart_beat(void *args) {
    server_heart_beat_arguments *heartbeat_args = args;
    int name_server_socket = socket_udp_name_server_socket(
            heartbeat_args->name_server_port, heartbeat_args->name_server_address);
    fprintf(stderr, "Trying to register to nameserver\n");

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        reg *reg = pdu_create_reg((int) strlen(heartbeat_args->own_server_name), heartbeat_args->own_port,
                                  heartbeat_args->own_server_name);
        socket_write_pdu_to((PDU *) reg, &name_server_socket, 1);

        ack *pdu_ack = NULL;

        //keep acking until nameserver accepts.
        while (pdu_ack == NULL) {
            pdu_ack = socket_read_ack_from_udp(name_server_socket);
            if (pdu_ack == NULL) {
                socket_write_pdu_to((PDU *) reg, &name_server_socket, 1);
            }
        }
        free(reg->server_name);
        free(reg);
        uint16_t id = pdu_ack->id_number;
        free(pdu_ack);
        fprintf(stderr, "Server is registered at name server.\n");
        not_reg *not_reg = NULL;
        while (not_reg == NULL) {
            alive *alive = pdu_create_alive(client_list_get_num_connected_clients(heartbeat_args->cl), id);
            socket_write_pdu_to((PDU *) alive, &name_server_socket, 1);
            free(alive);
            not_reg = socket_read_not_reg_from_udp(name_server_socket);
            sleep(2);

            //if pdu is ack instead of not_reg, ignore it.
            if (not_reg != NULL) {
                if (not_reg->pdu.op != 100) {
                    free(not_reg);
                    not_reg = NULL;
                } else{
                    free(not_reg);
                }
            }
        }
    }
#pragma clang diagnostic pop

}



