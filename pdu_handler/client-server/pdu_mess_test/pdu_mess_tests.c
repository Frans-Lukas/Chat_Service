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
    char* serialized_pdu;
    pdu_mess* pdu = pdu_mess_create(identity, message);
    htons(pdu->message_length);
    htonl(pdu->timestamp);
    pdu_mess_serialize((PDU *) pdu, &serialized_pdu);
    assert(serialized_pdu[0] == OP_MESS);
    assert(serialized_pdu[2] == strlen(identity));
    assert(create_checksum(pdu) == 0);
    assert((uint16_t)serialized_pdu[4] == strlen(message));
    assert(strcmp(&serialized_pdu[12], message) == 0);
    assert(strncmp(&serialized_pdu[12 + strlen(message) + 1], identity, strlen(identity)) == 0);

}

void assert_deserialize_pdu_mess_works() {
    char* identity = "An";
    char* message = "Hi.";
    int fd = open_fd("../pdu_handler/client-server/pdu_mess_test/data.pdu");
    int op;
    read_from_fd(fd, &op, 1);
    pdu_mess* deserialized_pdu = pdu_mess_deserialize(fd);
    htons(deserialized_pdu->message_length);
    htonl(deserialized_pdu->timestamp);
    assert(deserialized_pdu->op == OP_MESS);
    assert(deserialized_pdu->identity_length == strlen(identity));
    assert(deserialized_pdu->message_length == strlen(message));
    assert(strcmp((char*)deserialized_pdu->message, message) == 0);
    assert(strcmp((char*)deserialized_pdu->client_identity, identity) == 0);
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