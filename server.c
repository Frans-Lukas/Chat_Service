
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
    //run_all_tests();
    if(argc != 5){
        perror_exit("Wrong number of parameters");
    }
    //run_all_tests();
    server_run_server(atoi(argv[1]), argv[2], argv[3], atoi(argv[4]));
    return 0;
}