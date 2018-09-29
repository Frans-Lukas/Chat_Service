//
// Created by c16fld on 2018-09-26.
//

#ifndef CHAT_SERVICE_PDU_JOIN_TESTS_H
#define CHAT_SERVICE_PDU_JOIN_TESTS_H


#include <assert.h>
#include "pdu_helper.h"
#include "pdu_handler_client-server.h"
void assert_deserialize_pdu_join_works();
void run_pdu_join_tests();
void assert_pdu_join_create_works();
void assert_serialize_pdu_join_works();
#endif //CHAT_SERVICE_PDU_JOIN_TESTS_H
