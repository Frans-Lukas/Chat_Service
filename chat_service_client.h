//
// Created by c16fld on 2018-10-17.
//

#ifndef CHAT_SERVICE_CHAT_SERVICE_CLIENT_H
#define CHAT_SERVICE_CHAT_SERVICE_CLIENT_H

void init_client();
s_list* get_server_list_form_names_server(char *name_server_adress, int name_server_port);
void* connect_to_server(char *server_name, int port);

#endif //CHAT_SERVICE_CHAT_SERVICE_CLIENT_H
