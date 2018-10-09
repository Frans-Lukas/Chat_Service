//
// Created by c16fld on 2018-10-05.
//

#include <pdu_handler/pdu_helper.h>
#include "socket_interface.h"
#include "socket_helper.h"
#include <poll.h>


int write_pdu_to_socket(PDU* pdu, int socket[], int size){

    struct pollfd poll_struct[size];
    for(int i=0; i<size; i++){
        poll_struct[i].fd = socket[i];
        poll_struct[i].events = POLLOUT;
    }

    if( 0 > poll(poll_struct, 1, 100)){
        printf(stderr, "poll() error");
        return -1;
    }
    
    for(int i=0; i<size; i++) {
        if (poll_struct[i].revents & POLLOUT) {
            char *data;
            int pdu_size = pdu_serialize(pdu, &data);
            if(0 > socket_single_write_to(socket, data, pdu_size)){
                return -1;
            }
        }
    }
    return 0;
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
    if( 0 > poll(fd, (nfds_t) size, timeout)){
        printf(stderr, "poll() error");
        return NULL;
    }
    PDU* data[size];
    for (int j = 0; j < size; ++j) {
        if(fd[j].revents & POLLIN){
            data[j] = pdu_deserialize_next(socket[j]);
        } else{
            data[j] = NULL;
        }
    }
    return data;
}

