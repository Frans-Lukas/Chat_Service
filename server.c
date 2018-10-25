
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pdu_handler/pdu_helper.h"
#include "pdu_handler/pdu_tests.h"
#include "chat_service_server.h"
#include "chat_service_client.h"



/**
 * Chattservern tar 4 argument i följande ordning:
 * port
 * chattservernamn
 * namnserver-host
 * namnserver-port
 */


// ./Server 6969 Connect_and_get_rekt itchy.cs.umu.se 1337
int main(int argc, char *argv[]){
    if(argc != 5){
        perror_exit("Correct usage: [Port] [server name] [name-server] [name-server port]");
    }
    server_run_server(atoi(argv[1]), argv[2], argv[3], atoi(argv[4]));
    return 0;
}