//
// Created by c16fld on 2018-09-29.
//

#include "pdu_pleave_tests.h"

void run_pdu_pleave_tests() {
    assert_pdu_pleave_create_works();
    assert_deserialize_pdu_pleave_works();
    assert_serialize_pdu_pleave_works();
}


void assert_pdu_pleave_create_works(){
    pdu_pleave* pdu = pdu_pleave_create("Anna");
    assert(pdu->op == OP_PLEAVE);
    assert(pdu->identity_length == 4);
    assert(strcmp((char*)pdu->client_identity, "Anna") == 0);
    free(pdu);
}

void assert_serialize_pdu_pleave_works(){
    char* identity = "pepe";
    char* real_serialized_pdu;
    pdu_pleave_serialize((PDU *) pdu_pleave_create(identity), &real_serialized_pdu);
    assert(real_serialized_pdu[0] == OP_PLEAVE);
    assert(real_serialized_pdu[1] == 4);
    assert(strncmp(real_serialized_pdu + 8, identity, 4) == 0);
    free(real_serialized_pdu);
}

void assert_deserialize_pdu_pleave_works(){
    int fd = open_fd("../pdu_handler/client-server/pdu_pleave_test/pleave_data.pdu");
    int op_code;
    read_from_fd(fd, &op_code, 1);
    pdu_pleave* deserialized_pdu = pdu_pleave_deserialize(fd);
    //add same padding as mock pdu to make sure string compare works
    assert(deserialized_pdu->op == OP_PLEAVE);
    assert(deserialized_pdu->identity_length == 4);
    ntohl(deserialized_pdu->timestamp);
    assert(deserialized_pdu->timestamp == 5);
    assert(strncmp((char*)deserialized_pdu->client_identity, "pepe", 4) == 0);
    free(deserialized_pdu);
}