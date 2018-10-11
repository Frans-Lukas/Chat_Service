//
// Created by c16fld on 2018-10-05.
//

#ifndef CHAT_SERVICE_SOCKET_INTERFACE_H
#define CHAT_SERVICE_SOCKET_INTERFACE_H


int socket_write_pdu_to(PDU *pdu, int *socket, int socket_array_size);
PDU** socket_read_pdu_from(int *socket, int socket_array_size);


#endif //CHAT_SERVICE_SOCKET_INTERFACE_H
