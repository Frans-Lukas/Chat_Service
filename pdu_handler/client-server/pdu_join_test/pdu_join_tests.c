#include <zconf.h>
#include "pdu_join_tests.h"

void run_pdu_join_tests() {
    assert_pdu_join_create_works();
    assert_serialize_pdu_join_works();
    assert_deserialize_pdu_join_works();
}

void assert_pdu_join_create_works(){
    pdu_join* pdu = pdu_join_create("Anna");
    assert(pdu->op == OP_JOIN);
    assert(pdu->identity_length == 4);
    assert(strcmp((char*)pdu->identity, "Anna") == 0);
    free(pdu);
}

void assert_deserialize_pdu_join_works(){
//    char* string = "hello";
//    int fd = open_fd("../pdu_handler/client-server/pdu_join_test/data.pdu");
//    op_code op;
//    read_from_fd(fd, &op, 1);
//    int* data = calloc(1, 110);
//    for (int i = 0; i < 12; ++i) {
//        read(fd, data, 1);
//    }
//    pdu_join* deserialized_pdu = pdu_join_deserialize(fd);
//    assert(strcmp((char*)deserialized_pdu->identity, string) == 0);
//    free(deserialized_pdu);
//    close(fd);
}

void assert_serialize_pdu_join_works(){
    char* string = "hello";
    char* real_serialized_pdu;
    int size = pdu_join_serialize((PDU *) pdu_join_create(string), &real_serialized_pdu);
    //add same padding as mock pdu to make sure string compare works
    assert(size == 24);
    assert(real_serialized_pdu[0] == OP_JOIN);
    assert(real_serialized_pdu[1] == 5);
    assert(strncmp(&real_serialized_pdu[4], string, sizeof(string)) == 0);
    free(real_serialized_pdu);
}