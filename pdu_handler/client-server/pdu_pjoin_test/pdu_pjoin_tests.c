#include "pdu_pjoin_tests.h"


void run_pdu_pjoin_tests() {
    assert_pdu_pjoin_create_works();
    assert_serialize_pdu_pjoin_works();
    assert_deserialize_pdu_pjoin_works();
}


void assert_pdu_pjoin_create_works(){
    pdu_pjoin* pdu = pdu_pjoin_create("Anna");
    assert(pdu->op == OP_PJOIN);
    assert(pdu->identity_length == 4);
    assert(strcmp((char*)pdu->client_identity, "Anna") == 0);
    free(pdu);
}

void assert_serialize_pdu_pjoin_works(){
    char* identity = "pepe";
    char* real_serialized_pdu;
    pdu_pjoin_serialize((PDU *) pdu_pjoin_create(identity), &real_serialized_pdu);
    assert(real_serialized_pdu[0] == OP_PJOIN);
    assert(real_serialized_pdu[1] == 4);
    assert(strncmp(real_serialized_pdu + 8, identity, 4) == 0);
    free(real_serialized_pdu);
}

void assert_deserialize_pdu_pjoin_works(){
    char* identity = "pepe";
    int fd = open_fd("../pdu_handler/client-server/pdu_pjoin_test/data.pdu");
    int op;
    read_from_fd(fd, &op, 1);
    pdu_pjoin* deserialized_pdu = pdu_pjoin_deserialize(fd);
    //add same padding as mock pdu to make sure string compare works
    assert(deserialized_pdu->op == OP_PJOIN);
    assert(deserialized_pdu->identity_length == 4);
    assert(deserialized_pdu->timestamp == 5);
    assert(strcmp((char*)deserialized_pdu->client_identity, identity) == 0);
    free(deserialized_pdu);
}