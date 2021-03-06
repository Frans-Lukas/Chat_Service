#include <fcntl.h>
#include <zconf.h>
#include "pdu_quit_tests.h"

void run_pdu_quit_tests(){
    assert_pdu_quit_create_works();
    assert_serialize_pdu_quit_works();
}

void assert_serialize_pdu_quit_works() {
    char* mock_serialized_pdu = safe_calloc(1, sizeof(pdu_quit));
    mock_serialized_pdu[0] = OP_QUIT;
    char* real_serialized_pdu;
    int size = pdu_quit_serialize((PDU *) pdu_quit_create(), &real_serialized_pdu);
    assert(real_serialized_pdu[0] == OP_QUIT);
    assert(size == 4);
    free(real_serialized_pdu);
}

void assert_pdu_quit_create_works() {
    pdu_quit* pdu = pdu_quit_create();
    assert(pdu->pdu.op == OP_QUIT);
}

