

#include <socket_handler/socket_interface_test.h>
#include "pdu_tests.h"

void run_all_tests() {
    fprintf(stderr, "Starting tests...\n");
    run_pdu_join_tests();
    run_pdu_quit_tests();
    run_pdu_participants_tests();
    run_pdu_mess_tests();
    run_all_server_nameserver_tests();
    run_all_client_nameserver_test();
    run_pdu_pleave_tests();
    run_pdu_pjoin_tests();
    socket_interface_test_all();
    fprintf(stderr, "All tests successful!\n");
}