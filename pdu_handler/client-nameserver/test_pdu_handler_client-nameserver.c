
#include "test_pdu_handler_client-nameserver.h"


void run_all_client_nameserver_test() {
    test_serialize_s_list();
    test_deserialize_s_list();
}


void test_serialize_s_list() {
    s_list *pdu = calloc(1, sizeof(s_list));
    pdu->pdu.op = OP_SLIST;
    pdu->number_of_servers = 420;
    *(uint8_t *) (&pdu->adress) = 251;
    *((uint8_t *) (&pdu->adress) + 1) = 252;
    *((uint8_t *) (&pdu->adress) + 2) = 253;
    *((uint8_t *) (&pdu->adress) + 3) = 254;
    pdu->port = 7331;
    pdu->number_of_clients = 9;
    pdu->server_name_length = 8;
    pdu->server_name = calloc(2, sizeof(uint32_t));
    memcpy(pdu->server_name, "abcdefgh", 8);

    uint8_t *data = s_list_serialize(pdu);
    assert(data[0] == OP_SLIST);
    assert(data[1] == 0);
    assert(*(uint16_t *) (data + 2) == 420);
    assert(data[4] == 251);
    assert(data[5] == 252);
    assert(data[6] == 253);
    assert(data[7] == 254);
    assert(*(uint16_t *) (data + 8) == 7331);
    assert(data[10] == 9);
    assert(data[11] == 8);
}


void test_deserialize_s_list() {
    uint8_t *data = calloc(20, sizeof(uint8_t));
    data[0] = OP_SLIST;
    *(uint16_t *) (data + 2) = 123;
    *(data + 4) = 251;
    *(data + 5) = 252;
    *(data + 6) = 253;
    *(data + 7) = 254;
    *((uint16_t *) (data + 8)) = 1337;
    data[10] = 6;
    data[11] = 8;
    memcpy(&data[12], "abcdefgh", 8);
    s_list *pdu = s_list_deserialize(data);
    assert(pdu->pdu.op == OP_SLIST);
    assert(pdu->number_of_servers == 123);
    assert(*((uint8_t *) (&pdu->adress) + 0) == 251);
    assert(*((uint8_t *) (&pdu->adress) + 1) == 252);
    assert(*((uint8_t *) (&pdu->adress) + 2) == 253);
    assert(*((uint8_t *) (&pdu->adress) + 3) == 254);
    assert(pdu->port == 1337);
    assert(pdu->number_of_clients == 6);
    assert(pdu->server_name_length == 8);
    assert(strncmp((char*)pdu->server_name, "abcdefgh" ,8) == 0);
}
