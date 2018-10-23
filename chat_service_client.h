//
// Created by c16fld on 2018-10-17.
//

#ifndef CHAT_SERVICE_CHAT_SERVICE_CLIENT_H
#define CHAT_SERVICE_CHAT_SERVICE_CLIENT_H

#include <socket_handler/client_list.h>

typedef struct server_info{
    char* address;
    char* server_name;
    int port;
} server_info;

void init_client(char* username, char *server_option, char* server_adress, int server_port);
s_list* get_server_list_from_names_server(char *name_server_adress, int name_server_port);
void print_s_list(s_list *s);
char *format_to_ip(uint32_t *adress);
void handle_participants(pdu_participants *pdu);
void handle_quit(pdu_quit *pParticipants);

void handle_message(pdu_mess *pParticipants);

void handle_pjoin(pdu_pjoin *pParticipants);

void handle_pleave(pdu_pleave *pParticipants);
void print_message(pdu_mess *pdu);

server_info *let_user_choose_server(s_list *pList);
void send_join_to_server(client *client);
void print_user_message(pdu_mess* pdu);
void server_info_free(server_info *server_to_connect_to);

void client_free(client *client);
void* write_to_client_stdout(void* data);

void* read_from_client_stdin(void* data);

char* from_unix_to_human_time(time_t time);


#endif //CHAT_SERVICE_CHAT_SERVICE_CLIENT_H
