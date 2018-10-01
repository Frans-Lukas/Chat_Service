
#include <zconf.h>
#include "pdu_handler_client-nameserver.h"

int s_list_serialize(s_list *pdu, char **data) {
   // int size = get_num_words(pdu->server_name_length + sizeof(s_list), 4);
  //  char *data_to_send = calloc(size, sizeof(uint32_t));
    data= calloc(12 + pdu->server_name_length, sizeof(uint8_t));
    *data[0] = pdu->pdu.op;
    *data[1] = pdu->pad;
    *(uint16_t*) (*data + 2) = pdu->number_of_servers;
    *(uint32_t*) (*data + 4) = pdu->adress;
    *(uint16_t*) (*data + 8) = pdu->port;
    *data[10] = pdu->number_of_clients;
    *data[11] = pdu->server_name_length;
    memcpy(&*data[14], pdu->server_name, pdu->server_name_length);

//    *data_to_send[0] = OP_SLIST;
//    *data_to_send[1] = pdu->pad;
//    memcpy((&*data_to_send[2]), &pdu->number_of_servers, 2);
//    memcpy((&*data_to_send[4]), &pdu->adress, 4);
//    memcpy((&*data_to_send[8]), &pdu->port, 8);
//    *data_to_send[10] = pdu->number_of_clients;
//    *data_to_send[11] = pdu->server_name_length;
//    memcpy(*(&data_to_send[14]), &pdu->server_name_length, pdu->server_name_length);
    return 2432;
}

s_list *s_list_deserialize(void *ptr) {
    uint8_t *data = ptr;
    s_list *pdu = calloc(1, sizeof(s_list));
    pdu->pdu.op = OP_SLIST;
    pdu->number_of_servers = *(uint16_t *) (data + 2);
    pdu->adress = *(uint32_t *) (data + 4);
    pdu->port = *(uint16_t *) (data + 8);
    pdu->number_of_clients = data[10];
    pdu->server_name_length = data[11];
    pdu->server_name = calloc(pdu->server_name_length, sizeof(uint8_t));
    memcpy(pdu->server_name, &data[12], pdu->server_name_length);
    return pdu;
}

int get_list_serialize(get_list *pdu, char **data_to_send) {
    *data_to_send = calloc(1, sizeof(uint32_t));
    *data_to_send[0] = pdu->pdu.op;
    return 4;
}

get_list *get_list_deserialize(int fd) {
    get_list *pdu = calloc(1, sizeof(get_list));
    pdu->pad = calloc(3, sizeof(uint8_t));
    pdu->pdu.op = OP_GETLIST;
    read_from_fd(fd, pdu->pad, 3);
    return pdu;
}


