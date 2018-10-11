
#include <stdio.h>
#include <netinet/in.h>
#include "pdu_handler_server-nameserver.h"


int not_reg_serialize(PDU *p, char** data) {
    not_reg* pdu = (not_reg*)p;
    int size = 4;
    *data = calloc(4, sizeof(uint8_t));
    *data[0] = OP_NOTREG;
    pdu_cpy_chars(*data + 1, &pdu->pad, 0, 1);
    pdu->id_number = htons(pdu->id_number);
    pdu_cpy_chars(*data + 2, &pdu->id_number, 0, 2);
    return size;
}

not_reg *not_reg_deserialize(int fd) {
    not_reg *pdu = calloc(1, sizeof(not_reg));
    pdu->pdu.op = OP_NOTREG;
    read_from_fd(fd, &pdu->pad, 1);
    read_from_fd(fd, &pdu->id_number, 2);
    pdu->id_number = ntohs(pdu->id_number);
    return pdu;
}

int ack_serialize(PDU *p, char** data) {
    ack* pdu = (ack*)p;
    *data = calloc(4, sizeof(uint8_t));
    *data[0] = OP_ACK;
    *data[1] = pdu->pad;
    pdu_cpy_chars(*data + 1, &pdu->pad, 0, 1);
    pdu->id_number = htons(pdu->id_number);
    pdu_cpy_chars(*data + 2, &pdu->id_number, 0, 2);
    return 4;
}

ack *ack_deserialize(int fd) {
    ack *pdu = calloc(1, sizeof(ack));
    pdu->pdu.op = OP_ACK;
    read_from_fd(fd, &pdu->pad, 1);
    read_from_fd(fd, &pdu->id_number, 2);
    pdu->id_number = ntohs(pdu->id_number);
    return pdu;
}

int alive_serialize(PDU *p, char** data) {
    alive* pdu = (alive*)p;
    int size = sizeof(alive);
    *data = calloc(size, sizeof(uint8_t));
    *data[0] = OP_ALIVE;
    pdu_cpy_chars(*data + 1, &pdu->nr_of_clients, 0, 1);
    pdu->id_number = htons(pdu->id_number);
    pdu_cpy_chars(*data + 2, &pdu->id_number, 0, 2);
    return size;
}

alive *alive_deserialize(int fd) {
    alive *pdu = calloc(1, sizeof(alive));
    pdu->pdu.op = OP_ALIVE;
    read_from_fd(fd, &pdu->nr_of_clients, 1);
    read_from_fd(fd, &pdu->id_number, 2);
    pdu->id_number = ntohs(pdu->id_number);
    return pdu;
}

int reg_serialize(PDU *p, char** data) {
    reg* pdu = (reg*)p;
    int size = sizeof(reg) + pdu->server_name_length;
    *data = calloc(1, (size_t) size);
    *data[0] = OP_REG;
    pdu_cpy_chars((*data) + 1, &pdu->server_name_length, 0, 1);
    pdu->tcp_port = htons(pdu->tcp_port);
    pdu_cpy_chars((*data) + 2, &pdu->tcp_port, 0, 2);
    pdu_cpy_chars((*data) + 4, pdu->server_name, 0, pdu->server_name_length);
    return size;
}

reg* reg_deserialize(int fd) {
    reg* pdu = calloc(1, sizeof(reg));
    pdu->pdu.op = OP_REG;
    read_from_fd(fd, &pdu->server_name_length, 1);
    read_from_fd(fd, &pdu->tcp_port, 2);
    pdu->tcp_port = ntohs(pdu->tcp_port);
    int size = 4 + (get_num_words(pdu->server_name_length, 4) * 4);
    pdu->server_name = calloc((size_t) size, sizeof(uint32_t));
    read_from_fd(fd, pdu->server_name, pdu->server_name_length);
    return pdu;
}


