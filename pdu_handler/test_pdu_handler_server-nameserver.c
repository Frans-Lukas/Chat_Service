
#include <assert.h>
#include "test_pdu_handler_server-nameserver.h"
#include "pdu_handler_server-nameserver.h"
#include <string.h>


static char* str = "ab";
static int str_len = 6;
static int port = 1337;

void test_creating_reg_with_valid_arguments(){
    reg* r = create_REG(str, str_len, port);

    assert(r !=  NULL);
    assert(strcmp((char*)r->server_name, str) == 0);
    assert(r->tcp_port == 1337);
    assert(r->server_name_length == 6);
}


void test_deserialize(){
    char* pdu2 = calloc(10, 1);
    pdu2[0] = 0;
    pdu2[1] = 2;
    pdu2[2] = 57;
    pdu2[3] = 5;
    pdu2[4] = 'a';
    pdu2[5] = 'b';
    reg* r = reg_deserialize(pdu2);
    fprintf(stderr, "%d\n", r->tcp_port);
    assert(r !=  NULL);
    assert(strcmp((char*)r->server_name, str) == 0);
    assert(r->tcp_port == 1337);
    assert(r->server_name_length == 2);
}
