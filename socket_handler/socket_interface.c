//
// Created by c16fld on 2018-10-05.
//

#include <pdu_handler/pdu_helper.h>
#include "socket_interface.h"
#include "socket_helper.h"


int write_pdu_to_socket(PDU* pdu, int socket){
    char* data;
    pdu_serialize(pdu, &data);
    //if poll says socket is writable, write to socket.
    socket_single_write_to(socket, data);
}

PDU* read_pdu_from_socket(int socket){

    //if poll says socket is readable, read from socket.

    PDU* data = pdu_deserialize_next(socket);
    return data;
}