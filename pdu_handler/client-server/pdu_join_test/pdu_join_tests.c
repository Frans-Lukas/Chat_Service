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
    char* string = "hello";
    int fd = open_fd("../pdu_handler/client-server/pdu_join_test/data.pdu");
    op_code op;
    read_from_fd(fd, &op, 1);
    pdu_join* deserialized_pdu = pdu_join_deserialize(fd);
    assert(strcmp((char*)deserialized_pdu->identity, string) == 0);
    free(deserialized_pdu);
}

void assert_serialize_pdu_join_works(){
//    char* string = "hello";
//    char* mock_serialized_pdu = calloc(1, 1 + 1 + 2 + 8);
//    mock_serialized_pdu[0] = OP_JOIN;
//    mock_serialized_pdu[1] = 5;
//    mock_serialized_pdu[2] = 0;
//    mock_serialized_pdu[3] = 0;
//    mock_serialized_pdu[4] = 'h';
//    mock_serialized_pdu[5] = 'e';
//    mock_serialized_pdu[6] = 'l';
//    mock_serialized_pdu[7] = 'l';
//    mock_serialized_pdu[8] = 'o';
//    mock_serialized_pdu[9] = 'o';
//    mock_serialized_pdu[10] = 'o';
//    mock_serialized_pdu[11] = '\0';
//
//    char* real_serialized_pdu;
//    int size = pdu_join_serialize((PDU *) pdu_join_create(string), real_serialized_pdu);
//    //add same padding as mock pdu to make sure string compare works
//    real_serialized_pdu[9] = 'o';
//    real_serialized_pdu[10] = 'o';
//    real_serialized_pdu[11] = '\0';
//    assert(size == 12);
//    assert(strcmp(real_serialized_pdu, mock_serialized_pdu) == 0);
//    free(real_serialized_pdu);
//    free(mock_serialized_pdu);
}