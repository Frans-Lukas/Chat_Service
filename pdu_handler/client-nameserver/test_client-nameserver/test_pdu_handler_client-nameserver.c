
#include <zconf.h>
#include <netinet/in.h>
#include "test_pdu_handler_client-nameserver.h"


void run_all_client_nameserver_test() {
    test_deserialize_s_list();
    test_deserialize_get_list();
}


void test_serialize_s_list() {
//    s_list *pdu = safe_calloc(1, sizeof(s_list));
//    char *test_data;
//    pdu->pdu.op = OP_SLIST;
//    pdu->number_of_servers = 420;
//    *(uint8_t *) (&pdu->adress) = 251;
//    *((uint8_t *) (&pdu->adress) + 1) = 252;
//    *((uint8_t *) (&pdu->adress) + 2) = 253;
//    *((uint8_t *) (&pdu->adress) + 3) = 254;
//    pdu->port = 7331;
//    pdu->number_of_clients = 9;
//    pdu->server_name_length = 8;
//    pdu->server_name = safe_calloc(2, sizeof(uint32_t));
//    memcpy(pdu->server_name, "abcdefgh", 8);
//
//    int result = pdu_s_list_serialize(pdu, &test_data);
//
//    assert(test_data[0] == OP_SLIST);
//    assert(*(uint16_t *) (test_data + 2) == htons(420));
//    assert((uint8_t) test_data[4] == 251);
//    assert((uint8_t) test_data[5] == 252);
//    assert((uint8_t) test_data[6] == 253);
//    assert((uint8_t) test_data[7] == 254);
//    assert(*(uint16_t *) (test_data + 8) == htons(7331));
//    assert(test_data[10] == 9);
//    assert(test_data[11] == 8);
//    assert(strncmp((char *) pdu->server_name, "abcdefgh", 8) == 0);
//    assert(result == 20);
}


void test_deserialize_s_list() {
//    int fd = open_fd("../pdu_handler/client-nameserver/test_client-nameserver/s_list_data.pdu");
//    char c;
//    read_from_fd(fd, &c, 1);
//    s_list *pdu = pdu_s_list_deserialize(fd);
////
////    assert(pdu->pdu.op == OP_SLIST);
////    assert(pdu->number_of_servers == ntohs(2));
////
////    assert(*((uint8_t *) (&pdu->adress[0]) + 0) == 251);
////    assert(*((uint8_t *) (&pdu->adress[0]) + 1) == 252);
////    assert(*((uint8_t *) (&pdu->adress[0]) + 2) == 253);
////    assert(*((uint8_t *) (&pdu->adress[0]) + 3) == 254);
////    assert(pdu->port[0] == ntohs(1337));
////    //assert(pdu->number_of_clients[0] == 6);
////    //assert(pdu->server_name_length[0] == 8);
////    assert(strncmp((char*)(pdu->server_name[0]), "abcdefgh", 8) == 0);
////
////    assert(*((uint8_t *) (&pdu->adress[1]) + 0) == 100);
////    assert(*((uint8_t *) (&pdu->adress[1]) + 1) == 101);
////    assert(*((uint8_t *) (&pdu->adress[1]) + 2) == 102);
////    assert(*((uint8_t *) (&pdu->adress[1]) + 3) == 103);
////    assert(pdu->port[1] == ntohs(420));
////    assert(pdu->number_of_clients[1] == 5);
////    assert(pdu->server_name_length[1] == 4);
////    assert(strncmp((char*)(*(pdu->server_name) + 1), "efgh", 4) == 0);
}


void test_serialize_get_list() {
    char *test_data;
    get_list *test_pdu = safe_calloc(1, sizeof(get_list));
    test_pdu->pdu.op = OP_GETLIST;
    test_pdu->pad = safe_calloc(3, sizeof(uint8_t)), pdu_get_list_serialize((PDU *) test_pdu, &test_data);
    assert(test_data[0] == OP_GETLIST);
}


void test_deserialize_get_list() {
    int fd = open_fd("../pdu_handler/client-nameserver/test_client-nameserver/get_list_data.pdu");
    get_list *pdu = pdu_get_list_deserialize(fd);
    assert(pdu->pdu.op == OP_GETLIST);
}
