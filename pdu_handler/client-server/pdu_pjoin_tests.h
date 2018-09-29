//
// Created by c16fld on 2018-09-29.
//

#ifndef CHAT_SERVICE_PDU_PJOIN_TESTS_H
#define CHAT_SERVICE_PDU_PJOIN_TESTS_H

#include <assert.h>
#include "pdu_handler_client-server.h"

void run_pdu_pjoin_tests();
void assert_pdu_pjoin_create_works(void);
void assert_serialize_pdu_pjoin_works(void);
void assert_deserialize_pdu_pjoin_works(void);
#endif //CHAT_SERVICE_PDU_PJOIN_TESTS_H
