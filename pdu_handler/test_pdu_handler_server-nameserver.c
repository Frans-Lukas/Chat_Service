
#include "test_pdu_handler_server-nameserver.h"
#include "pdu_handler_server-nameserver.h"


void test_creating_reg_with_valid_arguments(){
    char* str = "hejsan";
    int str_len = 6;
    int port = 1337;


    if(create_REG(str, str_len, port) == NULL){
        fprintf(stderr, "create_REG returns NULL");
    }
}
