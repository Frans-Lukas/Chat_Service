//
// Created by c16fld on 2018-10-05.
//
#define _GNU_SOURCE


#include <pdu_handler/client-server/pdu_handler_client-server.h>
#include "socket_interface.h"
#include "client_list.h"

int socket_write_pdu_to(PDU *pdu, int *socket, int number_of_sockets) {
    for (int i = 0; i < number_of_sockets; i++) {
        char *data;
        int pdu_size = pdu_serialize(pdu, &data);

        if (0 > socket_single_write_to(socket[i], data, pdu_size)) {
            return -1;
        }
        free(data);

    }
    return 0;
}

PDU **socket_read_pdu_from(int *sockets, int number_of_sockets, client_list* cl) {
    //if poll says socket is readable, read from socket.
    struct pollfd fd[number_of_sockets];
    for (int i = 0; i < number_of_sockets; ++i) {
        fd[i].fd = sockets[i];
        fd[i].events = POLLIN | POLLRDHUP;
    }
    int timeout;
    timeout = 100;
    if (0 > poll(fd, (nfds_t) number_of_sockets, timeout)) {
        fprintf(stderr, "poll() error");
        return NULL;
    }
    PDU **data = safe_calloc((size_t) number_of_sockets, sizeof(PDU*));
    for (int j = 0; j < number_of_sockets; ++j) {
        if(fd[j].revents & POLLRDHUP && cl != NULL){
            disconnect_client_from_client_list(cl, j);
        } else if (fd[j].revents & POLLIN) {
            data[j] = pdu_deserialize_next(sockets[j]);
        } else{
            data[j] = NULL;
        }
    }
    return data;
}

void disconnect_client_from_client_list(client_list *cl, int j) {
    if(cl == NULL){
        return;
    }
    client connected_clients[client_list_get_num_connected_clients(cl)];
    int sockets[client_list_get_num_connected_clients(cl)];
    int number_of_sockets = 0;
    for (int i = 0; i < CLIENT_LIST_MAX_SIZE; ++i) {
                if(cl->clients[i].socket != 0){
                    connected_clients[number_of_sockets] = cl->clients[i];
                    sockets[number_of_sockets] = cl->clients[i].socket;
                    number_of_sockets++;
                }
            }
    if(connected_clients[j].identity != NULL){
                pdu_pleave* pleave = pdu_pleave_create(connected_clients[j].identity);
                socket_write_pdu_to((PDU *) pleave, sockets, number_of_sockets);
            }
    int socket_to_close = connected_clients[j].socket;
    client_list_remove_client(connected_clients[j], cl);
    close(socket_to_close);
    fprintf(stderr, "Disconnected client %s.\n", connected_clients[j].identity);
}

ack *socket_read_ack_from_udp(int socket) {
    ack *pdu_ack = NULL;
    struct pollfd fd[1];
    for (int i = 0; i < 1; ++i) {
        fd[i].fd = socket;
        fd[i].events = POLLIN;
    }
    int timeout;
    timeout = 1000;
    if (0 > poll(fd, (nfds_t) 1, timeout)) {
        fprintf(stderr, "poll() error");
        return NULL;
    }
    for (int j = 0; j < 1; ++j) {
        if (fd[j].revents & POLLIN) {
            pdu_ack = pdu_ack_deserialize(socket);
        } else {
            pdu_ack = NULL;
        }
    }
    return pdu_ack;
}

not_reg *socket_read_not_reg_from_udp(int socket) {
    not_reg *not_reg = NULL;
    struct pollfd fd[1];
    for (int i = 0; i < 1; ++i) {
        fd[i].fd = socket;
        fd[i].events = POLLIN;
    }
    int timeout;
    timeout = 2;
    if (0 > poll(fd, (nfds_t) 1, timeout)) {
        fprintf(stderr, "poll() error");
        return NULL;
    }
    for (int j = 0; j < 1; ++j) {
        if (fd[j].revents & POLLIN) {
            not_reg = pdu_not_reg_deserialize(socket);
        } else {
            not_reg = NULL;
        }
    }
    return not_reg;
}


int socket_tcp_server_create(int port) {
    int server_socket = socket_tcp_create();
    socket_bind(port, server_socket);
    socket_tcp_listen(server_socket);
    return server_socket;
}

int socket_tcp_client_create(int port, char *address) {
    int client_socket = socket_tcp_create();
    if (-1 == socket_connect(port, address, client_socket)) {
        return -1;
    }
    return client_socket;
}

int socket_udp_name_server_socket(int port, char *server_name) {
    int client_socket = socket_udp_create();
    char *ip = calloc(1, 255);
    network_hostname_to_ip(server_name, ip);
    socket_connect(port, ip, client_socket);
    free(ip);
    return client_socket;
}

int create_tcp_name_server_socket(int port, char *server_name) {
    int tcp_socket = socket_tcp_create();
    char *ip = safe_calloc(1, 17);
    network_hostname_to_ip(server_name, ip);

    if (0 != socket_connect(port, ip, tcp_socket)) {
        perror_exit("Could not connect to name_server");
    }
    free(ip);
    return tcp_socket;
}
