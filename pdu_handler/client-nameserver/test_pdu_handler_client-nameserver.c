
#include "test_pdu_handler_client-nameserver.h"


void run_all_client_nameserver_test(){
    test_serialize_s_list();
    test_deserialize_s_list();
}


void test_serialize_s_list() {
    s_list *pdu = calloc(1, sizeof(s_list));
    pdu->pdu.op = OP_SLIST;
    pdu->number_of_servers = 420;
    *(uint8_t*)(&pdu->adress) = 1;
    *((uint8_t*)(&pdu->adress) + 1) = 3;
    *((uint8_t*)(&pdu->adress) + 2) = 3;
    *((uint8_t*)(&pdu->adress) + 3) = 7;
    pdu->port = 7331;
    pdu->number_of_clients = 9;
    pdu->server_name_length = 8;
    pdu->server_name = calloc(2, sizeof(uint32_t));
    memcpy(pdu->server_name, "abcdefgh", 8);

    uint8_t *data = s_list_serialize(pdu);
    assert(data[0] == OP_SLIST);
    assert(data[1] == 0);
    assert(*(uint16_t*)(data + 2) == 420);
    assert(data[4] == 1);
    assert(data[5] == 3);
    assert(data[6] == 3);
    assert(data[7] == 7);
    assert(*(uint16_t*)(data + 8) == 7331);
    assert(data[10] == 9);
    assert(data[11] == 8);
}


void test_deserialize_s_list() {

}
