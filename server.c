
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pdu_handler/pdu_helper.h"
#include "pdu_handler/pdu_tests.h"
#include "chat_service_server.h"
#include "chat_service_client.h"

int main(){


    server_run_server(4242);
    return 0;
}