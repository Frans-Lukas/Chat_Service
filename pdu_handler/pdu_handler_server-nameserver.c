
#include <stdio.h>
#include <netinet/in.h>
#include "pdu_handler_server-nameserver.h"


void *not_reg_serialize(not_reg *pdu) {
    uint8_t *data = calloc(4, sizeof(uint8_t));
    data[0] = OP_NOTREG;
    data[1] = pdu->pad;
    *(uint16_t *) (data + 2) = pdu->id_number;
    return data;
}

not_reg *not_reg_deserialize(void *ptr) {
    uint8_t *tmp_ptr = ptr;
    not_reg *pdu = calloc(1, sizeof(not_reg));
    pdu->pdu.op = OP_NOTREG;
    pdu->pad = tmp_ptr[1];
    pdu->id_number = *(uint16_t *) (tmp_ptr + 2);
    return pdu;
}

void *ack_serialize(ack *pdu) {
    uint8_t *data = calloc(4, sizeof(uint8_t));
    data[0] = OP_ACK;
    data[1] = pdu->pad;
    *(uint16_t *) (data + 2) = pdu->id_number;
    return data;
}

ack *ack_deserilize(void *ptr) {
    uint8_t *tmp_ptr = ptr;
    ack *pdu = calloc(1, sizeof(ack));
    pdu->pdu.op = OP_ACK;
    pdu->pad = tmp_ptr[1];
    pdu->id_number = *(uint16_t *) (tmp_ptr + 2);
    return pdu;
}

void *alive_serialize(alive *pdu) {
    uint8_t *data = calloc(4, sizeof(uint8_t));
    data[0] = OP_ALIVE;
    data[1] = pdu->nr_of_clients;
    memcpy(&data[2], &pdu->id_number, sizeof(uint16_t));
    return data;
}

alive *alive_deserialize(void *ptr) {
    uint8_t *tmp_ptr = ptr;
    alive *pdu_alive = calloc(1, sizeof(alive));
    pdu_alive->pdu.op = OP_ALIVE;
    pdu_alive->nr_of_clients = tmp_ptr[1];
    pdu_alive->id_number = *(uint16_t *) (tmp_ptr + 2);
    return pdu_alive;
}

void *reg_serialize(reg *pdu) {
    uint8_t *data = calloc(4 + pdu->server_name_length, sizeof(uint8_t));
    data[0] = OP_REG;
    data[1] = pdu->server_name_length;
    memcpy(&data[2], &pdu->tcp_port, sizeof(uint16_t));
    memcpy(&data[4], pdu->server_name, pdu->server_name_length);
    return data;
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


