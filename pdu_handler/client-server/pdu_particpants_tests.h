#ifndef CHAT_SERVICE_PDU_PARTICPANTS_TESTS_H
#define CHAT_SERVICE_PDU_PARTICPANTS_TESTS_H
#include "pdu_helper.h"
#include "pdu_handler_client-server.h"
#include <assert.h>

void run_pdu_participants_tests();
void assert_pdu_participants_create_works();
void assert_serialize_pdu_participants_works();
void assert_deserialize_pdu_participants_works();

#endif //CHAT_SERVICE_PDU_PARTICPANTS_TESTS_H
