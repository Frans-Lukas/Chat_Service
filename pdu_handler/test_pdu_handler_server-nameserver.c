
#include "test_pdu_handler_server-nameserver.h"
#include "pdu_handler_server-nameserver.h"


void test_creating_reg(){
    char* str = "hejsan";
    int port = 1337;
    create_REG(str, port);
}