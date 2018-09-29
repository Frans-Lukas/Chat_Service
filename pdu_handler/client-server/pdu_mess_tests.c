//
// Created by c16fld on 2018-09-28.
//

#include "pdu_mess_tests.h"

void run_pdu_mess_tests(){
    assert_pdu_mess_create_works();
    //assert_serialize_pdu_mess_works();
   // assert_deserialize_pdu_mess_works();
}

void assert_deserialize_pdu_mess_works() {
    char* mock_serialized_pdu = calloc(1, );
    mock_serialized_pdu[0] = OP_QUIT;
    pdu_quit* deserialized_pdu = pdu_quit_deserialize(mock_serialized_pdu);
    assert(deserialized_pdu->op == OP_QUIT);
    free(mock_serialized_pdu);
    free(deserialized_pdu);
}

void assert_serialize_pdu_mess_works() {
    char* mock_serialized_pdu = calloc(1, 1);
    mock_serialized_pdu[0] = OP_QUIT;
    char* real_serialized_pdu = pdu_quit_serialize((PDU *) pdu_quit_create());
    assert(real_serialized_pdu[0] == OP_QUIT);
    free(real_serialized_pdu);
    free(mock_serialized_pdu);

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