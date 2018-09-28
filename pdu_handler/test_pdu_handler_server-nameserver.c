
#include <assert.h>
#include "test_pdu_handler_server-nameserver.h"
#include "pdu_handler_server-nameserver.h"
#include <string.h>

void run_all_reg_test() {
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

    uint8_t *data = not_reg_serialize(pdu);
    assert(data[0] == OP_NOTREG);
    assert(data[1] == 42);
    assert(*(uint16_t *) (data + 2) == 1337);
}

void test_deserialize_not_reg() {
    uint8_t *data = calloc(4, sizeof(uint8_t));
    data[0] = OP_NOTREG;
    data[1] = 42;
    *(uint16_t *) (data + 2) = 1337;

    not_reg *pdu = not_reg_deserialize(data);

    assert(pdu->pdu.op == OP_NOTREG);
    assert(pdu->pad == 42);
    assert(pdu->id_number == 1337);
}

void test_serialize_ack() {
    ack *pdu = calloc(1, sizeof(ack));
    pdu->pdu.op = OP_ACK;
    pdu->pad = 42;
    pdu->id_number = 1337;

    uint8_t *data = ack_serialize(pdu);
    assert(data[0] == OP_ACK);
    assert(data[1] == 42);
    assert(*(uint16_t *) (data + 2) == 1337);
}

void test_deserialize_ack() {
    uint8_t *data = calloc(4, sizeof(uint8_t));
    data[0] = OP_ACK;
    data[1] = 42;
    *(uint16_t *) (data + 2) = 1337;

    ack *pdu = ack_deserialize(data);

    assert(pdu->pdu.op == OP_ACK);
    assert(pdu->pad == 42);
    assert(pdu->id_number == 1337);
}

void test_serialize_alive() {
    alive *pdu = calloc(1, sizeof(alive));
    pdu->pdu.op = OP_ALIVE;
    pdu->id_number = (uint16_t) 1337;
    pdu->nr_of_clients = (uint8_t) 6;
    uint8_t *data = alive_serialize(pdu);

    assert(data[0] == OP_ALIVE);
    assert(data[1] == 6);
    assert(*(uint16_t *) (data + 2) == 1337);
}

void test_deserialize_alive() {
    uint8_t *data = calloc(4, sizeof(uint8_t));
    data[0] = OP_ALIVE;
    data[1] = 6;
    *((uint16_t *) (data + 2)) = 1337;

    alive *pdu = alive_deserialize(data);
    assert(pdu != NULL);
    assert(pdu->pdu.op == OP_ALIVE);
    assert(pdu->nr_of_clients == 6);
    assert(pdu->id_number == 1337);
}

void test_serialize_reg() {
    reg *pdu = calloc(1, sizeof(reg));
    pdu->pdu.op = OP_REG;
    pdu->server_name_length = 2;
    pdu->tcp_port = (uint16_t) 1337;
    pdu->server_name = calloc(1, sizeof(uint32_t));
    strncpy((char *) pdu->server_name, "ab", 2);

    uint8_t *data = reg_serialize(pdu);

    assert(data[0] == OP_REG);
    assert(data[1] == 2);
    assert(*((uint16_t *) (data + 2)) == 1337);
    assert(strncmp((char *) ((data + 4)), "ab", 2) == 0);
}

void test_deserialize_reg() {
    uint8_t *data = calloc(10, sizeof(uint8_t));
    data[0] = OP_REG;
    data[1] = 2;
    *((uint16_t *) (data + 2)) = 1337;
    strncpy((char *) &data[4], "ab", 2);

    reg *r = reg_deserialize(data);
    assert(r != NULL);
    assert(r->tcp_port == 1337);
    assert(r->server_name_length == 2);
    assert(strcmp((char *) r->server_name, "ab") == 0);
}
