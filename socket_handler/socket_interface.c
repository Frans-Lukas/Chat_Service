//
// Created by c16fld on 2018-10-05.
//

#include <pdu_handler/pdu_helper.h>
#include "socket_interface.h"
#include "socket_helper.h"
#include <poll.h>

int write_pdu_to_socket(PDU* pdu, int socket){
    char* data;
    pdu_serialize(pdu, &data);
    //if poll says socket is writable, write to socket.
    socket_single_write_to(socket, data);
}

PDU** read_pdu_from_socket(int socket[], int size){

    //if poll says socket is readable, read from socket.
    struct pollfd fd[size];
    for (int i = 0; i < size; ++i) {
        fd[i].fd = socket[i];
        fd[i].events = POLLIN;
    }
    int timeout;
    timeout = 100;
    poll(fd, (nfds_t) size, timeout);
    PDU* data[size];
    for (int j = 0; j < size; ++j) {
        if(fd[j].revents == POLLIN){
            data[j] = pdu_deserialize_next(socket[j]);
        } else{
            data[j] = NULL;
        }
    }
    return data;
}

