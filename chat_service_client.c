
#include <pdu_handler/client-nameserver/pdu_handler_client-nameserver.h>
#include "chat_service_client.h"
#include "socket_helper.h"
#include "socket_interface.h"
//
//När klienten startar kan den hämta en lista med servrar inklusive deras IP-adresser och portnummer från en
//namnserver. Sedan ska den ansluta till en av dessa servrar. Alternativt används inte namnservern utan
//klienten ansluter till en given server. När den är ansluten ska den kunna skicka och ta emot meddelanden,
//samt kunna lämna sessionen. Det finns inget krav på att kolla att en klientidentitet är unik i chattsessionen
//när en klient ansluter till chattsessione


void init_client(){
    char *name_server = "itchy.cs.umu.se";
    int port = 1337;
    s_list *server_list = get_server_list_form_names_server(name_server, port);
    fprintf(stderr, "%s", (char*)(&server_list->server_name[0]));
}


s_list* get_server_list_form_names_server(char *name_server_adress, int name_server_port){
    int server_name_socket = create_tcp_name_server_socket(name_server_port, name_server_adress);
    get_list* get_list = pdu_create_get_list();
    while(-1 == socket_write_pdu_to((PDU *) get_list, &server_name_socket, 1));

    s_list* server_list = NULL;
    while(NULL == (server_list = (s_list*)socket_read_pdu_from(&server_name_socket, 1)));

    return &server_list[0];
}

void* connect_to_server(char *server_name, int port) {
    int server_socket = socket_tcp_create();
    int client_socket = socket_connect(port, server_name, server_socket);
}

