#define _GNU_SOURCE

#ifndef CHAT_SERVICE_SOCKET_INTERFACE_H

#define CHAT_SERVICE_SOCKET_INTERFACE_H
#include <pdu_handler/server-nameserver/pdu_handler_server-nameserver.h>
#include "../error_handling.h"
#include <poll.h>
#include <signal.h>
#include <pdu_handler/server-nameserver/pdu_handler_server-nameserver.h>
#include <pdu_handler/pdu_helper.h>
#include "socket_helper.h"
#include "client_list.h"

int socket_write_pdu_to(PDU *pdu, int *socket, int number_of_sockets);
PDU **socket_read_pdu_from(int *sockets, int number_of_sockets, client_list* cl);
int create_client_socket(int adress, char *hostname);
int socket_server_connect(int adress, char* hostname);
int socket_tcp_server_create(int port);
int socket_udp_name_server_socket(int port, char* server_name);
not_reg* socket_read_not_reg_from_udp(int socket);
int socket_tcp_client_create(int port, char* address);
void disconnect_client_from_client_list(client_list *cl, int index);
int create_tcp_name_server_socket(int port, char* server_name);
#endif //CHAT_SERVICE_SOCKET_INTERFACE_H
