#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pdu_handler/pdu_helper.h"
#include "pdu_handler/pdu_tests.h"
<<<<<<< HEAD
#include "chat_service_client.h"

int main() {
    //run_all_tests();
    init_client();
=======
#include "chat_service_server.h"

int main() {
    run_all_tests();
    server_run_server(3003);
>>>>>>> 751a6bd6349b6e57863c41088ecc7dc41032fd5a
    return 0;
}