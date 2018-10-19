
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
    print_s_list(server_list);
    
}


void print_s_list(s_list* s){



    fprintf(stderr, "op code =  %d\n", s->pdu.op);
    fprintf(stderr, "number of servers = %d\n\n\n", s->number_of_servers);


    for(int i=0; i < s->number_of_servers; i++){
        fprintf(stderr, "adress[%d] = %d.%d.%d.%d\n", i, *((uint8_t *) (&s->adress[i]) + 0), *((uint8_t *) (&s->adress[i]) + 1),
                *((uint8_t *) (&s->adress[i]) + 2), *((uint8_t *) (&s->adress[i]) + 3));
        fprintf(stderr, "port[%d] = %d\n", i, s->port[i]);
        fprintf(stderr, "number_of_clients[%d] = %d\n", i, s->number_of_clients[i]);
        fprintf(stderr, "server_name_length[%d] = %d\n", i, s->server_name_length[i]);
        fprintf(stderr, "%s",(char*)s->server_name[i]);
        fprintf(stderr, "\n\n\n");
    }

}

char *format_to_ip(uint32_t *adress){
    char *kuba = calloc(16, sizeof(char));
    sprintf(kuba, "%d.%d.%d.%d\n", *((uint8_t *) adress + 0), *((uint8_t *) adress + 1),
            *((uint8_t *)adress + 2), *((uint8_t *) adress) + 3);
    return kuba;
}


s_list* get_server_list_form_names_server(char *name_server_adress, int name_server_port){
    int server_name_socket = create_tcp_name_server_socket(name_server_port, name_server_adress);
    get_list* get_list = pdu_create_get_list();
    while(-1 == socket_write_pdu_to((PDU *) get_list, &server_name_socket, 1));

    PDU** response = NULL;
    while(NULL == (response = socket_read_pdu_from(&server_name_socket, 1)));

    return (s_list *) response[0];
}


void* connect_to_server(char *server_name, int port) {
    int server_socket = socket_tcp_create();
    int client_socket = socket_connect(port, server_name, server_socket);
}


