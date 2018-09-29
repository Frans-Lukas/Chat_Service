
#include "pdu_handler_client-nameserver.h"


void *s_list_serialize(s_list *pdu) {
    uint8_t* data = calloc(12 + pdu->server_name_length, sizeof(uint8_t));
    data[0] = pdu->pdu.op;
    data[1] = pdu->pad;
    *(uint16_t*) (data + 2) = pdu->number_of_servers;
    *(uint32_t*) (data + 4) = pdu->adress;
    *(uint16_t*) (data + 8) = pdu->port;
    data[10] = pdu->number_of_clients;
    data[11] = pdu->server_name_length;
    memcpy(&data[14], pdu->server_name, pdu->server_name_length);
    return data;
}

s_list *s_list_deserialize(void *ptr) {
    uint8_t *data = ptr;
    s_list *pdu = calloc(1, sizeof(s_list));
    pdu->pdu.op = OP_SLIST;
    pdu->number_of_servers = *(uint16_t*) (data + 2);
    pdu->adress = *(uint32_t*) (data + 4);
    pdu->port = *(uint16_t*) (data + 8);
    pdu->number_of_clients = data[10];
    pdu->server_name_length = data[11];
    pdu->server_name = calloc(pdu->server_name_length, sizeof(uint8_t));
    memcpy(pdu->server_name, &data[12], pdu->server_name_length);
    return pdu;
}

void *get_list_serialize(get_list* pdu){
    uint8_t *data = calloc(1, sizeof(uint32_t));
    data[0] = pdu->pdu.op;
    return data;
}

get_list* get_list_deserialize(void* ptr){
    get_list* pdu = calloc(1, sizeof(get_list));
    pdu->pdu.op = OP_GETLIST;
    pdu->pad = calloc(3, sizeof(uint8_t));
    return pdu;
}

