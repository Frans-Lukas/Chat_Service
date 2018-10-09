//
// Created by c16fld on 2018-10-05.
//

#ifndef CHAT_SERVICE_SOCKET_INTERFACE_H
#define CHAT_SERVICE_SOCKET_INTERFACE_H


int write_pdu_to_socket(PDU* pdu, int socket[], int size);
PDU** read_pdu_from_socket(int socket[], int size);


#endif //CHAT_SERVICE_SOCKET_INTERFACE_H
