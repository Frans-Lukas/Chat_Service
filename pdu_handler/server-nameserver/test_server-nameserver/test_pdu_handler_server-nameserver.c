
#include <assert.h>
#include "test_pdu_handler_server-nameserver.h"
#include "pdu_handler_server-nameserver.h"
#include <string.h>
#include <zconf.h>
#include <netinet/in.h>

void run_all_server_nameserver_tests() {
    test_deserialize_reg();
    test_serialize_reg();

    test_serialize_alive();
    test_deserialize_alive();

    test_serialize_ack();
    test_deserialize_ack();

    test_serialize_not_reg();
    test_deserialize_not_reg();
}

void test_serialize_not_reg() {
    not_reg *pdu = calloc(1, sizeof(not_reg));
    pdu->pdu.op = OP_NOTREG;
    pdu->pad = 42;
    pdu->id_number = 1337;

    char *data;
    pdu_not_reg_serialize(pdu, &data);
    assert(data[0] == OP_NOTREG);
    assert(data[1] == 42);
    assert(*(uint16_t *) (data + 2) == htons(1337));
}

void test_deserialize_not_reg() {
    int fd = open_fd("../pdu_handler/server-nameserver/test_server-nameserver/not_reg_data.pdu");
    int op_code;
    read_from_fd(fd, &op_code, 1);
    not_reg *pdu = pdu_not_reg_deserialize(fd);
    assert(pdu->pdu.op == OP_NOTREG);
    assert(pdu->pad == 42);
    assert(pdu->id_number == ntohs(1337));
}

void test_serialize_ack() {
    ack *pdu_ack = calloc(1, sizeof(ack));
    pdu_ack->pdu.op = OP_ACK;
    pdu_ack->pad = 42;
    pdu_ack->id_number = 1337;

    char *data;
    pdu_ack_serialize(pdu_ack, &data);
    assert(data[0] == OP_ACK);
    assert(data[1] == 42);
    assert(*(uint16_t *) (data + 2) == htons(1337));
}

void test_deserialize_ack() {
    int fd = open_fd("../pdu_handler/server-nameserver/test_server-nameserver/ack_data.pdu");
    int op_code;
    read_from_fd(fd, &op_code, 1);
    ack *pdu = pdu_ack_deserialize(fd);
    assert(pdu->pdu.op == OP_ACK);
    assert(pdu->pad == 42);
    assert(pdu->id_number == ntohs(1337));
}

void test_serialize_alive() {
    alive *pdu = calloc(1, sizeof(alive));
    pdu->pdu.op = OP_ALIVE;
    pdu->id_number = (uint16_t) 1337;
    pdu->nr_of_clients = (uint8_t) 6;
    char *data;
    pdu_alive_serialize(pdu, &data);

    assert(data[0] == OP_ALIVE);
    assert(data[1] == 6);
    assert(*(uint16_t *) (data + 2) == htons(1337));
}

void test_deserialize_alive() {
    int fd = open_fd("../pdu_handler/server-nameserver/test_server-nameserver/alive_data.pdu");
    int op_code;
    read_from_fd(fd, &op_code, 1);
    alive *pdu = pdu_alive_deserialize(fd);
    assert(pdu->pdu.op == OP_ALIVE);
    assert(pdu != NULL);
    assert(pdu->pdu.op == OP_ALIVE);
    assert(pdu->nr_of_clients == 6);
    assert(pdu->id_number == ntohs(1337));
}

void test_serialize_reg() {
    reg *pdu = calloc(1, sizeof(reg));
    pdu->pdu.op = OP_REG;
    pdu->server_name_length = 2;
    pdu->tcp_port = (uint16_t) 1337;
    pdu->server_name = calloc(1, sizeof(uint32_t));
    strncpy((char *) pdu->server_name, "ab", 2);

    char *data;
    int result = pdu_reg_serialize(pdu, &data);

    assert(result == 18);
    assert(data[0] == OP_REG);
    assert(data[1] == 2);
    assert(*((uint16_t *) (data + 2)) == htons(1337));
    assert(strncmp((data + 4), "ab", 2) == 0);
}

void test_deserialize_reg() {
    int fd = open_fd("../pdu_handler/server-nameserver/test_server-nameserver/reg_data.pdu");
    int op_code;
    read_from_fd(fd, &op_code, 1);
    reg *pdu = pdu_reg_deserialize(fd);
    assert(pdu->pdu.op == OP_REG);
    assert(pdu != NULL);
    assert(pdu->tcp_port == ntohs(1337));
    assert(pdu->server_name_length == 2);
    assert(strcmp((char *) pdu->server_name, "ab") == 0);
    close(fd);
}
