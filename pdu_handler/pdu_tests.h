
#ifndef CHAT_SERVICE_PDU_TESTS_H
#define CHAT_SERVICE_PDU_TESTS_H

#include <assert.h>
#include "pdu_handler/client-server/pdu_handler_client-server.h"
#include "pdu_handler/client-server/pdu_join_tests.h"
#include "pdu_handler/client-server/pdu_quit_tests.h"
#include "pdu_handler/client-server/pdu_pleave_tests.h"
#include "pdu_handler/client-server/pdu_pjoin_tests.h"
#include "pdu_handler/client-server/pdu_mess_tests.h"
#include "pdu_handler/client-server/pdu_particpants_tests.h"
#include "server-nameserver/pdu_handler_server-nameserver.h"
#include "server-nameserver/test_pdu_handler_server-nameserver.h"
#include <pdu_handler/client-nameserver/test_pdu_handler_client-nameserver.h>


void run_all_tests();

#endif //CHAT_SERVICE_PDU_TESTS_H
