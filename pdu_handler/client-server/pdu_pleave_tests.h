//
// Created by c16fld on 2018-09-29.
//

#ifndef CHAT_SERVICE_PDU_PLEAVE_TESTS_H
#define CHAT_SERVICE_PDU_PLEAVE_TESTS_H
#include <assert.h>
#include "pdu_handler_client-server.h"

void assert_pdu_pleave_create_works(void);
void assert_deserialize_pdu_pleave_works(void);
void assert_serialize_pdu_pleave_works(void);
void run_pdu_pleave_tests(void);
#endif //CHAT_SERVICE_PDU_PLEAVE_TESTS_H
