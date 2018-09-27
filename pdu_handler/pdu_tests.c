
#include "test_pdu_handler_server-nameserver.h"
#include "pdu_tests.h"
#include "pdu_handler_server-nameserver.h"

void run_all_tests(){
    fprintf(stderr, "Starting tests...\n");
    assert_serialize_pdu_join_works();
    assert_deserialize_pdu_join_works();
    assert_invalid_pdu_join_is_invalid();
    assert_valid_pdu_join_is_valid();
    assert_data_validation_is_valid_on_data_divisible_by_four();
    assert_data_validation_is_invalid_on_data_not_divisible_by_four();
    run_all_reg_test();
    fprintf(stderr, "All tests successful!\n");
}

void assert_data_validation_is_valid_on_data_divisible_by_four() {
    char* divisible_by_four = "four";
    assert(data_is_divisible_by_four(divisible_by_four) == true);
}

void assert_data_validation_is_invalid_on_data_not_divisible_by_four() {
    char* divisible_by_four = "one";
    assert(data_is_divisible_by_four(divisible_by_four) == false);
}

void assert_deserialize_pdu_join_works(){
    char* string = "hellooooo";
    void* serialized_pdu = pdu_join_create(string);
    void* pdu = pdu_join_serialize(serialized_pdu);
    pdu_join* deserialized_pdu = pdu_join_deserialize(pdu);
    fprintf(stderr, "%s\n", (char*)deserialized_pdu->identity);
    assert(strcmp((char*)deserialized_pdu->identity, string) == 0);
}


void assert_serialize_pdu_join_works(){
    char* string = "hellooooo";
    void* serialized_pdu = pdu_join_serialize((PDU *) pdu_join_create(string));
    assert(strcmp((char*)serialized_pdu+4, string) == 0);
}

void assert_invalid_pdu_join_is_invalid(){
    pdu_join* pdu = pdu_join_create("Identity");
    pdu->op = OP_QUIT;
    assert(pdu_join_is_valid(pdu) == false);
}

void assert_valid_pdu_join_is_valid(){
    pdu_join* pdu = pdu_join_create("Identity");
    assert(pdu_join_is_valid(pdu) == true);
}

