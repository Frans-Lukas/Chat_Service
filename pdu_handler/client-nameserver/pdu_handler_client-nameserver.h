
#ifndef CHAT_SERVICE_PDU_HANDLER_CLIENT_NAMESERVER_H
#define CHAT_SERVICE_PDU_HANDLER_CLIENT_NAMESERVER_H

#include <pdu_handler/pdu_helper.h>

#define OP_GETLIST 3
#define OP_SLIST 4

typedef struct get_list {
    PDU pdu;
    uint8_t *pad;
} get_list;

typedef struct s_list {
    PDU pdu;
    uint8_t pad;
    uint16_t number_of_servers;
    uint32_t* adress;
    uint16_t* port;
    uint8_t* number_of_clients;
    uint8_t* server_name_length;
    uint32_t** server_name;
} s_list;

s_list *s_list_deserialize(int fd);

int s_list_serialize(s_list *pdu, char** data_to_send);

get_list *get_list_deserialize(int fd);

int get_list_serialize(get_list* pdu, char** data_to_send);

#endif //CHAT_SERVICE_PDU_HANDLER_CLIENT_NAMESERVER_H
