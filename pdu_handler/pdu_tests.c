//
// Created by c16fld on 2018-09-26.
//
#include "pdu_tests.h"

void run_all_tests(){
    fprintf(stderr, "Starting tests...\n");
    run_pdu_join_tests();
    run_pdu_quit_tests();
    run_pdu_participants_tests();
    run_pdu_mess_tests();
    fprintf(stderr, "All tests successful!\n");
}