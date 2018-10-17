
#ifndef CHAT_SERVICE_SOCKET_INTERFACE_H
#define CHAT_SERVICE_SOCKET_INTERFACE_H
#include "../error_handling.h"

int socket_write_pdu_to(PDU *pdu, int *socket, int number_of_sockets);
PDU** socket_read_pdu_from(int *socket, int number_of_socketst);
int create_client_socket(int adress, char *hostname);
int socket_server_connect(int adress, char* hostname);


#endif //CHAT_SERVICE_SOCKET_INTERFACE_H
