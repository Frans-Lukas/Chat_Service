#include "pdu_quit_tests.h"

void run_pdu_quit_tests(){
    assert_pdu_quit_create_works();
    assert_serialize_pdu_quit_works();
    assert_deserialize_pdu_quit_works();
}

void assert_deserialize_pdu_quit_works() {
    char* mock_serialized_pdu = calloc(1, 1);
    mock_serialized_pdu[0] = OP_QUIT;
    pdu_quit* deserialized_pdu = pdu_quit_deserialize(mock_serialized_pdu);
    assert(deserialized_pdu->op == OP_QUIT);
    free(mock_serialized_pdu);
    free(deserialized_pdu);
}

void assert_serialize_pdu_quit_works() {
    char* mock_serialized_pdu = calloc(1, 1);
    mock_serialized_pdu[0] = OP_QUIT;
    char* real_serialized_pdu = pdu_quit_serialize((PDU *) pdu_quit_create());
    assert(real_serialized_pdu[0] == OP_QUIT);
    free(real_serialized_pdu);
    free(mock_serialized_pdu);

}

void assert_pdu_quit_create_works() {
    pdu_quit* pdu = pdu_quit_create();
    assert(pdu->op == OP_QUIT);
}

