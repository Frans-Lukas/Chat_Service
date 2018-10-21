//
// Created by c16fld on 2018-10-17.
//

#ifndef CHAT_SERVICE_CHAT_SERVICE_CLIENT_H
#define CHAT_SERVICE_CHAT_SERVICE_CLIENT_H

typedef struct server_info{
    char* address;
    char* server_name;
    int port;
} server_info;


typedef struct client_info{
    int server_socket;
} client_info;

void init_client(char* username, char *server_option, char* server_adress, int server_port);
s_list* get_server_list_form_names_server(char *name_server_adress, int name_server_port);
void print_s_list(s_list *s);
char *format_to_ip(uint32_t *adress);
void handle_response(pdu_participants *pdu );
void handle_quit(pdu_quit *pParticipants);

void handle_message(pdu_mess *pParticipants);

void handle_pjoin(pdu_pjoin *pParticipants);

void handle_pleave(pdu_pleave *pParticipants);
void print_message(pdu_mess *pdu);

server_info *let_user_choose_server(s_list *pList);
void write_to_client_stdout(int server_socket);
void send_join_to_server(int server_socket);


void read_from_client_stdin(client_info *client);




#endif //CHAT_SERVICE_CHAT_SERVICE_CLIENT_H
