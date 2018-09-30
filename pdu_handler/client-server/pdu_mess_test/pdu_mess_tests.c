//
// Created by c16fld on 2018-09-28.
//

#include "pdu_mess_tests.h"

void run_pdu_mess_tests(){
    assert_pdu_mess_create_works();
    assert_serialize_pdu_mess_works();
    assert_deserialize_pdu_mess_works();
}

void assert_serialize_pdu_mess_works(){
    char* identity = "Anders";
    char* message = "Hejsan!";
    char* serialized_pdu = pdu_mess_serialize((PDU *) pdu_mess_create(identity, message));
    assert(serialized_pdu[0] == OP_MESS);
    assert(serialized_pdu[2] == strlen(identity));
    assert(serialized_pdu[3] == create_checksum(message));
    assert((uint16_t)serialized_pdu[4] == strlen(message));
    assert(strcmp(&serialized_pdu[12], message) == 0);
    assert(strncmp(&serialized_pdu[12 + strlen(message) + 1], identity, strlen(identity)) == 0);

}

void assert_deserialize_pdu_mess_works() {
    char* identity = "Anna";
    char* message = "Halloj.";
    char* mock_serialized_pdu = calloc(1, sizeof(pdu_mess) + strlen(message) + strlen(identity));
    mock_serialized_pdu[0] = OP_MESS;
    mock_serialized_pdu[2] = (char) strlen(identity);
    mock_serialized_pdu[3] = create_checksum(message);
    mock_serialized_pdu[4] = 7;
    mock_serialized_pdu[8] = 5;
    mock_serialized_pdu[12] = 'H';
    mock_serialized_pdu[13] = 'a';
    mock_serialized_pdu[14] = 'l';
    mock_serialized_pdu[15] = 'l';
    mock_serialized_pdu[16] = 'o';
    mock_serialized_pdu[17] = 'j';
    mock_serialized_pdu[18] = '.';
    mock_serialized_pdu[19] = '\0';
    mock_serialized_pdu[20] = 'A';
    mock_serialized_pdu[21] = 'n';
    mock_serialized_pdu[22] = 'n';
    mock_serialized_pdu[23] = 'a';
    mock_serialized_pdu[24] = '\0';
    pdu_mess* deserialized_pdu = pdu_mess_deserialize(mock_serialized_pdu);
    assert(deserialized_pdu->op == OP_MESS);
    assert(deserialized_pdu->identity_length == strlen(identity));
    assert(deserialized_pdu->checksum == create_checksum(message));
    assert(deserialized_pdu->message_length == strlen(message));
    assert(deserialized_pdu->timestamp == 5);
    assert(strcmp((char*)deserialized_pdu->message, message) == 0);
    assert(strcmp((char*)deserialized_pdu->client_identity, identity) == 0);
    free(mock_serialized_pdu);
    free(deserialized_pdu);

}

void assert_pdu_mess_create_works() {
    char* identity = "Tester";
    char* message = "Hello, world!";
    pdu_mess* pdu = pdu_mess_create(identity, message);
    assert(pdu->op == OP_MESS);
    assert(pdu->identity_length == strlen(identity));
    assert(pdu->message_length == strlen(message));
    assert(strncmp((char*)pdu->message, message, pdu->message_length) == 0);
    assert(strncmp((char*)pdu->client_identity, identity, pdu->identity_length) == 0);
}