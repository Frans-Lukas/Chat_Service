//
// Created by c16fld on 2018-10-05.
//

#include <pdu_handler/pdu_helper.h>
#include "socket_interface.h"
#include "socket_helper.h"
#include <poll.h>


int socket_write_pdu_to(PDU *pdu, int *socket, int number_of_sockets) {

    struct pollfd poll_struct[number_of_sockets];
    for (int i = 0; i < number_of_sockets; i++) {
        poll_struct[i].fd = socket[i];
        poll_struct[i].events = POLLOUT;
    }

    if (0 > poll(poll_struct, 1, 100)) {
        printf(stderr, "poll() error");
        return -1;
    }

    for (int i = 0; i < number_of_sockets; i++) {
        if (poll_struct[i].revents & POLLOUT) {
            char *data;
            int pdu_size = pdu_serialize(pdu, &data);
            if (0 > socket_single_write_to(socket[i], data, pdu_size)) {
                return -1;
            }
        }
    }
    return 0;
}

PDU** socket_read_pdu_from(int *socket, int number_of_sockets) {

    //if poll says socket is readable, read from socket.
    struct pollfd fd[number_of_sockets];
    for (int i = 0; i < number_of_sockets; ++i) {
        fd[i].fd = socket[i];
        fd[i].events = POLLIN;
    }
    int timeout;
    timeout = 2;
    if (0 > poll(fd, (nfds_t) number_of_sockets, timeout)) {
        printf(stderr, "poll() error");
        return NULL;
    }
    PDU** data = safe_calloc((size_t) number_of_sockets, sizeof(PDU));
    for (int j = 0; j < number_of_sockets; ++j) {
        if(fd[j].revents & POLLIN){
            data[j] = pdu_deserialize_next(socket[j]);
        } else{
            data[j] = NULL;
        }
    }
    return data;
}

int socket_name_server_tcp_connect(int adress, char *hostname) {
    socket_tcp_create();


    return 0;
}

