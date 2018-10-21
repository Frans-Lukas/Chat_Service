
#include <pdu_handler/client-nameserver/pdu_handler_client-nameserver.h>
#include <pdu_handler/client-server/pdu_handler_client-server.h>
#include <pthread.h>
#include "chat_service_client.h"
#include "socket_helper.h"
#include "socket_interface.h"
//
//När klienten startar kan den hämta en lista med servrar inklusive deras IP-adresser och portnummer från en
//namnserver. Sedan ska den ansluta till en av dessa servrar. Alternativt används inte namnservern utan
//klienten ansluter till en given server. När den är ansluten ska den kunna skicka och ta emot meddelanden,
//samt kunna lämna sessionen. Det finns inget krav på att kolla att en klientidentitet är unik i chattsessionen
//när en klient ansluter till chattsessione






void init_client(char* username, char *server_option, char* server_adress, int server_port){
    char *name_server = "itchy.cs.umu.se";
    int port = 1337;
    s_list *server_list = get_server_list_form_names_server(name_server, port);
    server_info* server_to_connect_to = let_user_choose_server(server_list);
    fprintf(stderr, "port: %d\n", server_to_connect_to->port);
    int server_socket = socket_tcp_client_create(server_to_connect_to->port, server_to_connect_to->address);



    if(server_socket == -1){
        perror_exit("could not connect to server.");
    }

    fprintf(stderr, "Connected to server\n");

    client_info *client = calloc(1, sizeof(client_info));

    client->server_socket = server_socket;


    send_join_to_server(server_socket);


    while(1){

        read_from_client_stdin(client);
        write_to_client_stdout(server_socket);

    }

}


int read_from_client_stdin(client_info *client){
    //char *text = calloc(1,1);
    char buffer[255];

    printf("Enter a message: ");
    while( fgets(buffer, 255 , stdin) ) /* break with ^D or ^Z */
    {
        //text = realloc( text, strlen(text)+1+strlen(buffer) );
        //if( !text ) {
            //ERROR
        //}
        //strcat( text, buffer ); /* note a '\n' is appended here everytime */
        //printf("%s\n", buffer);
        char *identity = "kuba";
        pdu_mess * mess = pdu_mess_create(identity, buffer);
        if(socket_write_pdu_to((PDU*)mess, &client->server_socket, 1) == -1){
            fprintf(stderr, "socket_write_pdu_to mess failed\n");
        }
    }
    //printf("\ntext:\n%s",text);
}

int write_to_client_stdout(int server_socket){
    PDU** response = NULL;
    while (response  == NULL || response[0] == NULL){
        response = socket_read_pdu_from(&server_socket, 1);
    }

    switch (response[0]->op){
        case OP_MESS:
            fprintf(stderr, "Got Message\n");
            handle_message((pdu_mess *) response[0]);
            break;
        case OP_QUIT:
            fprintf(stderr, "Got quit\n");
            handle_quit((pdu_quit *) response[0]);
            break;
        case OP_PJOIN:
            fprintf(stderr, "Got pjoin\n");
            handle_pjoin((pdu_pjoin *) response[0]);
            break;
        case OP_PLEAVE:
            fprintf(stderr, "Got pleave\n");
            handle_pleave((pdu_pleave *) response[0]);
            break;
        case OP_PARTICIPANTS:
            fprintf(stderr, "Got participants\n");
            handle_response((pdu_participants*) response[0]);
            break;
        default:
            fprintf(stderr, "Something is fishy\n");
            break;
    }
    sleep(1);
}




void chat_session(){

    //Medans sessionen är alive

    //Läs meddelande ifall det finns något att läsa

    // Skriv meddelande ifall det finns något att skriva


}


void send_join_to_server(int server_socket){
    char *name = "Kuba";
    pdu_join* pdu = pdu_join_create(name);
    while(-1 == socket_write_pdu_to((PDU*)pdu, &server_socket, 1));
}


void handle_pleave(pdu_pleave *pdu) {

}

void handle_pjoin(pdu_pjoin *pdu) {

}



void handle_message(pdu_mess *pdu) {
    print_message(pdu);
}

void print_message(pdu_mess *pdu){
    fprintf(stderr, "\n\n\nMESSAGE RECEIVED");

    fprintf(stderr, "identity_length: %d\n", pdu->identity_length);
    fprintf(stderr, "check_sum: %d\n", pdu->checksum);
    fprintf(stderr, "message_length: %d\n", pdu->message_length);
    fprintf(stderr, "padding_message_length: %d\n", pdu->padding_message_length);
    fprintf(stderr, "time_stamp: %d\n", pdu->timestamp);

    fprintf(stderr, "message: ");
    for(int i=0; i<pdu->message_length; i++){
        fprintf(stderr, "%c", pdu->message[i]);
    }
    fprintf(stderr, "\n");
    fprintf(stderr, "client_identity: ");
    for(int i=0; i<pdu->identity_length; i++){
        fprintf(stderr, "%c", pdu->client_identity[i]);
    }
    fprintf(stderr, "\n\n\n");
}

void handle_quit(pdu_quit *pdu) {

}


server_info *let_user_choose_server(s_list *pList) {
    fprintf(stderr, "Please choose a server to connect to.\n");
    for (int i = 0; i < pList->number_of_servers; ++i) {
        fprintf(stderr, "%d. %s\n", i, (char*)pList->server_name[i]);
    }
    int choice;
    scanf("%d", &choice);
    fprintf(stderr, "Trying to connect to server %s\n.", (char*)pList->server_name[choice]);
    server_info* server_to_connect_to = calloc(1, sizeof(server_info));
    server_to_connect_to->server_name = (char *) pList->server_name[choice];
    server_to_connect_to->port = pList->port[choice];
    server_to_connect_to->address = format_to_ip(&pList->adress[choice]);

    return server_to_connect_to;
}

void handle_response(pdu_participants *pdu) {

    fprintf(stderr, "Number of identities: %d\n",pdu->num_identities);

    for(int i=0; i<pdu->num_identities; i++){
        fprintf(stderr, "%s\n" , (char *) pdu->participant_names);
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

