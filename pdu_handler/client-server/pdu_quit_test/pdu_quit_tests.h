//
// Created by c16fld on 2018-09-26.
//

#ifndef CHAT_SERVICE_PDU_QUIT_TESTS_H
#define CHAT_SERVICE_PDU_QUIT_TESTS_H
#include "pdu_helper.h"
#include "pdu_handler_client-server.h"
#include <assert.h>

void run_pdu_quit_tests();
void assert_pdu_quit_create_works();
void assert_serialize_pdu_quit_works();
void assert_deserialize_pdu_quit_works();
#endif //CHAT_SERVICE_PDU_QUIT_TESTS_H
