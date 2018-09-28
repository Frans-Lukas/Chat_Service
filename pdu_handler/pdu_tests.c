
#include "server-nameserver/test_pdu_handler_server-nameserver.h"

#include "pdu_tests.h"
#include "server-nameserver/pdu_handler_server-nameserver.h"

void run_all_tests(){
    fprintf(stderr, "Starting tests...\n");
    run_pdu_join_tests();
    run_pdu_quit_tests();
    run_pdu_participants_tests();
    run_all_server_nameserver_tests();
    fprintf(stderr, "All tests successful!\n");
}
