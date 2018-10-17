//
// Created by c16fld on 2018-10-05.
//

#ifndef CHAT_SERVICE_SOCKET_INTERFACE_H
#define CHAT_SERVICE_SOCKET_INTERFACE_H
#include "../error_handling.h"

int socket_write_pdu_to(PDU *pdu, int *socket, int number_of_sockets);
PDU** socket_read_pdu_from(int *socket, int number_of_socketst);


#endif //CHAT_SERVICE_SOCKET_INTERFACE_H
