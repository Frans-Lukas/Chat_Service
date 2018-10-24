
#include <pdu_handler/client-nameserver/pdu_handler_client-nameserver.h>
#include <pdu_handler/client-server/pdu_handler_client-server.h>
#include <pthread.h>
#include <errno.h>
#include "chat_service_client.h"
#include "socket_helper.h"
#include "socket_interface.h"
//
//När klienten startar kan den hämta en lista med servrar inklusive deras IP-adresser och portnummer från en
//namnserver. Sedan ska den ansluta till en av dessa servrar. Alternativt används inte namnservern utan
//klienten ansluter till en given server. När den är ansluten ska den kunna skicka och ta emot meddelanden,
//samt kunna lämna sessionen. Det finns inget krav på att kolla att en klientidentitet är unik i chattsessionen
//när en klient ansluter till chattsessione



/**
 * TODO
 * Client-Disconnect
 * SKickar OP.code 0
 * Fixa alla fakkin errors
 */




// ./client kubalito ns nameserver.cs.umu.se 1337
void init_client(char* username, char *server_option, char* server_adress, int server_port){
    int server_socket = 0;

    if(strcmp(server_option , "ns") == 0){
        s_list *server_list = get_server_list_form_names_server(server_adress, server_port);
        server_info* server_to_connect_to = let_user_choose_server(server_list);
        server_socket = socket_tcp_client_create(server_to_connect_to->port, server_to_connect_to->address);
        s_list_free(server_list);
        server_info_free(server_to_connect_to);
    } else {
        char *ip = calloc(256, sizeof(char));
        network_hostname_to_ip(server_adress, ip);
        server_socket = socket_tcp_client_create(server_port, ip);
        free(ip);
    }


    if(server_socket == -1){
        perror_exit("could not connect to server.");
    }

    fprintf(stdout, "Connected to server\n");

    client client = {0};
    client.identity = username;
    client.socket = server_socket;

    send_join_to_server(&client);

    pthread_t reader_thread;


    pthread_create(&reader_thread, NULL, &read_from_client_stdin, &client);
    write_to_client_stdout(&server_socket);

    pthread_join(reader_thread, NULL);
}

void client_free(client *client) {
    free(client->identity);
    free(client);
}

void server_info_free(server_info *server_to_connect_to) {
    //free(server_to_connect_to->server_name);
    free(server_to_connect_to->address);
    free(server_to_connect_to);
}


void* read_from_client_stdin(void* data){
    client *client = data;
    while(1) {
        char buffer[255];
        while (fgets(buffer, 255, stdin)) /* break with ^D or ^Z */
        {
            if(strcmp(buffer, "\n") == 0){
                continue;
            }
            pdu_mess *mess = pdu_mess_create(client->identity, buffer);
            if (socket_write_pdu_to((PDU *) mess, &client->socket, 1) == -1) {

                fprintf(stderr, "socket_write_pdu_to mess failed\n");
                fprintf(stderr, " Value of errno: %d\n ", errno);
                return NULL;
            }
        }
    }
}

void* write_to_client_stdout(void* data){
    int server_socket = *(int*)data;

    while(1) {
        PDU **response = NULL;
        while (response == NULL || response[0] == NULL) {
            response = socket_read_pdu_from(&server_socket, 1);
        }
        switch (response[0]->op) {
            case OP_MESS:
                handle_message((pdu_mess *) response[0]);
                break;
            case OP_QUIT:
                handle_quit((pdu_quit *) response[0]);
                return NULL;
            case OP_PJOIN:
                handle_pjoin((pdu_pjoin *) response[0]);
                break;
            case OP_PLEAVE:
                handle_pleave((pdu_pleave *) response[0]);
                break;
            case OP_PARTICIPANTS:
                handle_participants((pdu_participants *) response[0]);
                break;
            default:
                break;
        }
    }
}

void send_join_to_server(client *client){
    pdu_join* pdu = pdu_join_create(client->identity);
    while(-1 == socket_write_pdu_to((PDU*)pdu, &client->socket, 1));
}

void handle_pleave(pdu_pleave *pdu) {
    char* to_print = calloc(1, pdu->identity_length + 1);
    strncat(to_print, (char *) pdu->client_identity, pdu->identity_length);
    fprintf(stdout, "Client %s left the server.\n", (char *) pdu->client_identity);
}

void handle_pjoin(pdu_pjoin *pdu) {
    char* to_print = calloc(1, pdu->identity_length + 1);
    strncat(to_print, (char *) pdu->client_identity, pdu->identity_length);
    fprintf(stdout, "Client %s joined the server.\n", to_print);
}

void handle_message(pdu_mess *pdu) {
    print_user_message(pdu);
}

char* from_unix_to_human_time(time_t time){
    struct tm  ts;
    char * buf = calloc(80, sizeof(char));
    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = *localtime(&time);
    strftime(buf, sizeof(buf), "%H:%M", &ts);
    return buf;
}

void print_user_message(pdu_mess *pdu){
    if(pdu->timestamp == 0 || pdu->message_length > 65535){
        perror("Invalid message recieved\n");
    } else{
        //print_message(pdu);
        fprintf(stdout, "\n[%s] %s : %s", from_unix_to_human_time(pdu->timestamp), (char *) pdu->client_identity,
                (char *) pdu->message);
    }
}


void print_message(pdu_mess *pdu){
    fprintf(stderr, "\n\n\nMESSAGE RECEIVED\n");

    fprintf(stderr, "identity_length: %d\n", pdu->identity_length);
    fprintf(stderr, "check_sum: %d\n", pdu->checksum);
    fprintf(stderr, "message_length: %d\n", pdu->message_length);
    fprintf(stderr, "padding_message_length: %d\n", pdu->padding_message_length);
    fprintf(stderr, "time_stamp: %d\n", pdu->timestamp);

    fprintf(stderr, "message: ");
    fprintf(stderr, "%s", (char*)pdu->message);
    fprintf(stderr, "client_identity: ");
    fprintf(stderr, "%s", (char*)pdu->client_identity);

    fprintf(stderr, "\n\n\n");
}

void handle_quit(pdu_quit *pdu) {

}


server_info *let_user_choose_server(s_list *pList) {
    fprintf(stdout, "Please choose a server to connect to.\n");
    for (int i = 0; i < pList->number_of_servers; ++i) {
        fprintf(stdout, "%d. ", i);
        for (int j = 0; j < pList->server_name_length[i]; ++j) {
            fprintf(stdout, "%c", ((char*)pList->server_name[i])[j]);
        }
        fprintf(stdout, "\n");
    }

    int choice;
    scanf("%d", &choice);
    fprintf(stdout, "Trying to connect to server ");
    for (int j = 0; j < pList->server_name_length[choice]; ++j) {
        fprintf(stdout, "%c", ((char*)pList->server_name[choice])[j]);
    }
    fprintf(stdout, "\n");
    server_info* server_to_connect_to = calloc(1, sizeof(server_info));
    server_to_connect_to->server_name = (char *) pList->server_name[choice];
    server_to_connect_to->port = pList->port[choice];
    server_to_connect_to->address = format_to_ip(&pList->adress[choice]);

    return server_to_connect_to;
}

void handle_participants(pdu_participants *pdu) {
    fprintf(stdout, "Joined server with participants:\n");
    char* currptr = (char *) pdu->participant_names;
    for(int i=0; i<pdu->num_identities; i++){
        fprintf(stdout, "%s\n" , currptr);
        currptr += strlen(currptr) + 1;
    }
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
    char *kuba = calloc(18, sizeof(uint8_t));
    sprintf(kuba, "%d.%d.%d.%d", *((uint8_t *) adress + 0), *((uint8_t *) adress + 1),
            *((uint8_t *)adress + 2), *((uint8_t *) adress + 3));
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

