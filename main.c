#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pdu_handler/pdu_helper.h"
#include "pdu_handler/pdu_tests.h"
#include "chat_service_server.h"
#include "chat_service_client.h"

int main() {
    //run_all_tests();
    server_run_server(3003);
    //init_client();
    return 0;
}