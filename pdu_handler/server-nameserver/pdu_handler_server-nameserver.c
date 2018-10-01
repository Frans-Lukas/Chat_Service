
#include <stdio.h>
#include <netinet/in.h>
#include "pdu_handler_server-nameserver.h"


int not_reg_serialize(not_reg *pdu, char** data) {
    int size = 4;
    *data = calloc(4, sizeof(uint8_t));
    *data[0] = OP_NOTREG;
    pdu_cpy_chars(*data + 1, &pdu->pad, 0, 1);
    pdu_cpy_chars(*data + 2, &pdu->id_number, 0, 2);
    return size;
}

not_reg *not_reg_deserialize(void *ptr) {
    uint8_t *tmp_ptr = ptr;
    not_reg *pdu = calloc(1, sizeof(not_reg));
    pdu->pdu.op = OP_NOTREG;
    pdu->pad = tmp_ptr[1];
    pdu->id_number = *(uint16_t *) (tmp_ptr + 2);
    return pdu;
}

int ack_serialize(ack *pdu, char** data) {
    *data = calloc(4, sizeof(uint8_t));
    *data[0] = OP_ACK;
    *data[1] = pdu->pad;
    pdu_cpy_chars(*data + 1, &pdu->pad, 0, 1);
    pdu_cpy_chars(*data + 2, &pdu->id_number, 0, 2);
    return 4;
}

ack *ack_deserialize(void *ptr) {
    uint8_t *tmp_ptr = ptr;
    ack *pdu = calloc(1, sizeof(ack));
    pdu->pdu.op = OP_ACK;
    pdu->pad = tmp_ptr[1];
    pdu->id_number = *(uint16_t *) (tmp_ptr + 2);
    return pdu;
}

int alive_serialize(alive *pdu, char** data) {
    int size = sizeof(alive);
    *data = calloc(size, sizeof(uint8_t));
    *data[0] = OP_ALIVE;
    pdu_cpy_chars(*data + 1, &pdu->nr_of_clients, 0, 1);
    pdu_cpy_chars(*data + 2, &pdu->id_number, 0, 2);
    return size;
}

alive *alive_deserialize(void *ptr) {
    uint8_t *tmp_ptr = ptr;
    alive *pdu_alive = calloc(1, sizeof(alive));
    pdu_alive->pdu.op = OP_ALIVE;
    pdu_alive->nr_of_clients = tmp_ptr[1];
    pdu_alive->id_number = *(uint16_t *) (tmp_ptr + 2);
    return pdu_alive;
}

int reg_serialize(reg *pdu, char** data) {
    int size = sizeof(reg) + pdu->server_name_length;
    *data = calloc(1, (size_t) size);
    *data[0] = OP_REG;
    pdu_cpy_chars((*data) + 1, &pdu->server_name_length, 0, 1);
    pdu_cpy_chars((*data) + 2, &pdu->tcp_port, 0, 2);
    pdu_cpy_chars((*data) + 4, pdu->server_name, 0, pdu->server_name_length);
    return size;
}

reg *reg_deserialize(void *ptr) {
    uint8_t *tmp_ptr = ptr;
    reg *pdu_reg = calloc(1, sizeof(reg));
    pdu_reg->pdu.op = OP_REG;
    memcpy(&pdu_reg->server_name_length, &tmp_ptr[1], sizeof(uint8_t));
    memcpy(&pdu_reg->tcp_port, &tmp_ptr[2], sizeof(uint16_t));
    pdu_reg->server_name = build_words((char *) &tmp_ptr[4], 4);
    return pdu_reg;
}


