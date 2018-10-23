
#include <stdio.h>
#include <netinet/in.h>
#include "pdu_handler_server-nameserver.h"

reg *pdu_create_reg(int server_name_length, int tcp_port, char *server_name) {
    reg *pdu = calloc(1, sizeof(reg));
    pdu->pdu.op = OP_REG;
    pdu->server_name_length = (uint8_t) server_name_length;
    pdu->tcp_port = (uint16_t) tcp_port;
    pdu->server_name = build_words(server_name, 4, pdu->server_name_length);
    return pdu;
}

alive *pdu_create_alive(int number_of_clients, int id_number) {
    alive* pdu = calloc(1, sizeof(alive));
    pdu->pdu.op = OP_ALIVE;
    pdu->nr_of_clients = (uint8_t) number_of_clients;
    pdu->id_number = (uint16_t) id_number;
    return pdu;
}

not_reg *pdu_create_not_reg(int id_number) {
    not_reg* pdu = calloc(1, sizeof(not_reg));
    pdu->pdu.op = OP_NOTREG;
    pdu->id_number = (uint16_t) id_number;
    return pdu;
}

ack *pdu_create_ack(int id_number) {
    ack* pdu = calloc(1, sizeof(ack));
    pdu->pdu.op = OP_ACK;
    pdu->id_number = (uint16_t) id_number;
    return pdu;
}

int pdu_not_reg_serialize(PDU *p, char **data) {
    not_reg* pdu = (not_reg*)p;
    int size = 4;
    *data = safe_calloc(4, sizeof(uint8_t));
    *data[0] = OP_NOTREG;
    pdu_cpy_chars(*data + 1, &pdu->pad, 0, 1);
    pdu->id_number = htons(pdu->id_number);
    pdu_cpy_chars(*data + 2, &pdu->id_number, 0, 2);
    return size;
}

not_reg *pdu_not_reg_deserialize(int fd) {
    not_reg *pdu = safe_calloc(1, sizeof(not_reg));
    char* data = calloc(1, 4);
    read_from_fd(fd, data, 4);
    pdu->pdu.op = (op_code) data[0];
    pdu_cpy_chars(&pdu->id_number, data, 2, 2);
    pdu->id_number = ntohs(pdu->id_number);
    free(data);
    return pdu;
}

int pdu_ack_serialize(PDU *p, char **data) {
    ack* pdu = (ack*)p;
    *data = safe_calloc(4, sizeof(uint8_t));
    *data[0] = OP_ACK;
    *data[1] = pdu->pad;
    pdu_cpy_chars(*data + 1, &pdu->pad, 0, 1);
    pdu->id_number = htons(pdu->id_number);
    pdu_cpy_chars(*data + 2, &pdu->id_number, 0, 2);
    return 4;
}

ack *pdu_ack_deserialize(int fd) {
    ack *pdu = safe_calloc(1, sizeof(ack));

    char* data = calloc(1, 4);
    read_from_fd(fd, data, 4);
    pdu->pdu.op = OP_ACK;
    pdu_cpy_chars(&pdu->id_number, data, 2, 2);
    pdu->id_number = ntohs(pdu->id_number);
    free(data);
    return pdu;
}

int pdu_alive_serialize(PDU *p, char **data) {
    alive* pdu = (alive*)p;
    int size = sizeof(alive);
    *data = safe_calloc(size, sizeof(uint8_t));
    *data[0] = OP_ALIVE;
    pdu_cpy_chars(*data + 1, &pdu->nr_of_clients, 0, 1);
    pdu->id_number = htons(pdu->id_number);
    pdu_cpy_chars(*data + 2, &pdu->id_number, 0, 2);
    return size;
}

alive *pdu_alive_deserialize(int fd) {
    alive *pdu = safe_calloc(1, sizeof(alive));
    pdu->pdu.op = OP_ALIVE;
    read_from_fd(fd, &pdu->nr_of_clients, 1);
    read_from_fd(fd, &pdu->id_number, 2);
    pdu->id_number = ntohs(pdu->id_number);
    return pdu;
}

int pdu_reg_serialize(PDU *p, char **data) {
    reg* pdu = (reg*)p;
    int size = 4 + get_num_words(pdu->server_name_length, 4) * 4;
    *data = safe_calloc(1, (size_t) size);
    *data[0] = OP_REG;
    pdu_cpy_chars((*data) + 1, &pdu->server_name_length, 0, 1);
    uint16_t* port = calloc(1, 2);
    pdu_cpy_chars(port, &pdu->tcp_port, 0, 2);
    *port = htons(*port);
    //port = htons(port);
    pdu_cpy_chars((*data) + 2, port, 0, 2);
    free(port);
    uint32_t * temp = build_words((char *) pdu->server_name, 4, pdu->server_name_length);
    pdu_cpy_chars((*data) + 4, temp, 0, pdu->server_name_length);
    free(temp);
    return size;
}

reg* pdu_reg_deserialize(int fd) {
    reg* pdu = safe_calloc(1, sizeof(reg));
    pdu->pdu.op = OP_REG;
    read_from_fd(fd, &pdu->server_name_length, 1);
    read_from_fd(fd, &pdu->tcp_port, 2);
    pdu->tcp_port = ntohs(pdu->tcp_port);
    int size = get_num_words(pdu->server_name_length, 4);
    pdu->server_name = safe_calloc((size_t) size, sizeof(uint32_t));
    read_from_fd(fd, pdu->server_name, pdu->server_name_length);
    return pdu;
}


