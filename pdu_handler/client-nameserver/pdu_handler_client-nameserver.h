
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
    uint32_t* address;
    uint16_t* port;
    uint8_t* number_of_clients;
    uint8_t* server_name_length;
    uint32_t** server_name;
} s_list;

get_list* pdu_create_get_list();

s_list *pdu_s_list_deserialize(int fd);

int s_list_free(s_list* list);

int pdu_s_list_serialize(PDU *p, char **data_to_send);

get_list *pdu_get_list_deserialize(int fd);

int pdu_get_list_serialize(PDU *p, char **data_to_send);

#endif //CHAT_SERVICE_PDU_HANDLER_CLIENT_NAMESERVER_H
