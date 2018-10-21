#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pdu_handler/pdu_helper.h"
#include "pdu_handler/pdu_tests.h"
#include "chat_service_server.h"
#include "chat_service_client.h"


//Chattklienten tar 4 argument i följande ordning:
//identitet (t.ex. ett nickname)
//  "ns" (för namnserver) eller "cs" (för chattserver)
//  namnserver-host eller chattserver-host
//  namnsserver-port eller chattserver-port

// ./client kubalito ns nameserver.cs.umu.se 1337
int main(int argc, char *argv[]) {

//    if(argc != 5){
//        fprintf(stderr, "Wrong amount of parameters");
//    }
   // run_all_tests();
    init_client(argv[1], argv[2], argv[3], atoi(argv[4]));
    return 0;
}