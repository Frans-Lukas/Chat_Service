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
    char* real_serialized_pdu = pdu_pleave_serialize((PDU *) pdu_pleave_create(identity));
    assert(real_serialized_pdu[0] == OP_PLEAVE);
    assert(real_serialized_pdu[1] == 4);
    assert(strncmp(real_serialized_pdu + 8, identity, 4) == 0);
    free(real_serialized_pdu);
}

void assert_deserialize_pdu_pleave_works(){
    char* identity = "pepe";
    char* mock_serialized_pdu = calloc(1, sizeof(pdu_pleave) + strlen(identity) + 1);
    mock_serialized_pdu[0] = OP_PLEAVE;
    mock_serialized_pdu[1] = 4;
    mock_serialized_pdu[4] = 5;
    mock_serialized_pdu[8] = 'p';
    mock_serialized_pdu[9] = 'e';
    mock_serialized_pdu[10] = 'p';
    mock_serialized_pdu[11] = 'e';
    mock_serialized_pdu[12] = '\0';
    mock_serialized_pdu[13] = '\0';
    mock_serialized_pdu[14] = '\0';
    mock_serialized_pdu[15] = '\0';

    pdu_pleave* deserialized_pdu = pdu_pleave_deserialize(mock_serialized_pdu);
    //add same padding as mock pdu to make sure string compare works
    assert(deserialized_pdu->op == OP_PLEAVE);
    assert(deserialized_pdu->identity_length == 4);
    assert(deserialized_pdu->timestamp == 5);
    assert(strcmp((char*)deserialized_pdu->client_identity, identity) == 0);
    free(mock_serialized_pdu);
    free(deserialized_pdu);
}