
#include <zconf.h>
#include "pdu_handler_client-nameserver.h"

int s_list_serialize(s_list *pdu, char **data) {
    int size = 12 + (get_num_words(pdu->server_name_length, 4) * 4);
    *data = calloc(1, sizeof(s_list));
    *data[0] = OP_SLIST;
    pdu_cpy_chars(*data + 2, &pdu->number_of_servers, 0, 2);
    pdu_cpy_chars(*data + 4, &pdu->adress, 0, 4);
    pdu_cpy_chars(*data + 8, &pdu->port, 0, 2);
    pdu_cpy_chars(*data + 10, &pdu->number_of_clients, 0, 1);
    pdu_cpy_chars(*data + 11, &pdu->server_name_length, 0, 1);
    pdu_cpy_chars(*data + 12, &pdu->server_name, 0, pdu->server_name_length);
    return size;
}

s_list *s_list_deserialize(int fd) {
    s_list *pdu = calloc(1, sizeof(s_list));
    pdu->pdu.op = OP_SLIST;
    read_from_fd(fd, &pdu->pdu.op, 1);
    read_from_fd(fd, &pdu->pad, 1);
    read_from_fd(fd, &pdu->number_of_servers, 2);
    pdu->server_name = calloc(pdu->server_name_length, sizeof(uint8_t));
    read_from_fd(fd, &pdu->adress, 4);
    read_from_fd(fd, &pdu->port, 2);
    read_from_fd(fd, &pdu->number_of_clients, 1);
    read_from_fd(fd, &pdu->server_name_length, 1);
    read_from_fd(fd, pdu->server_name, pdu->server_name_length);
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


