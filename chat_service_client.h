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

void init_client();
s_list* get_server_list_form_names_server(char *name_server_adress, int name_server_port);
void print_s_list(s_list *s);
char *format_to_ip(uint32_t *adress);
#endif //CHAT_SERVICE_CHAT_SERVICE_CLIENT_H
