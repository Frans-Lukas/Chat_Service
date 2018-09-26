//
// Created by c16fld on 2018-09-26.
//

#ifndef CHAT_SERVICE_PDU_TESTS_H
#define CHAT_SERVICE_PDU_TESTS_H
#include <assert.h>
#include "pdu_handler_client-server.h"


void assert_serialize_pdu_join_works();
void run_all_tests();
void assert_deserialize_pdu_join_works();
void assert_invalid_pdu_join_is_invalid();
void assert_valid_pdu_join_is_valid();
void assert_data_validation_is_invalid_on_data_not_divisible_by_four();
void assert_data_validation_is_valid_on_data_divisible_by_four();
void assert_pdu_join_is_divisible_by_four();

#endif //CHAT_SERVICE_PDU_TESTS_H
