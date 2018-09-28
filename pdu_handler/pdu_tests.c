
#include "test_pdu_handler_server-nameserver.h"
//
// Created by c16fld on 2018-09-26.
//
#include "pdu_tests.h"
#include "pdu_handler_server-nameserver.h"

void run_all_tests(){
    fprintf(stderr, "Starting tests...\n");
    run_pdu_join_tests();
    run_pdu_quit_tests();
    run_pdu_participants_tests();
    run_all_reg_test();
    fprintf(stderr, "All tests successful!\n");
}
