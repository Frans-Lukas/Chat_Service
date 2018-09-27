
#include <assert.h>
#include "test_pdu_handler_server-nameserver.h"
#include "pdu_handler_server-nameserver.h"
#include <string.h>


static char* server_name = "ab";
static int server_name_length = 2;
static int port = 1337;

void run_all_reg_test(){
    test_deserialize();
    test_creating_reg_with_valid_arguments();
}

void test_creating_reg_with_valid_arguments(){
    reg* r = create_REG(server_name, server_name_length, port);

    assert(r !=  NULL);
    assert(strcmp((char*)r->server_name, server_name) == 0);
    assert(r->tcp_port == 1337);
    assert(r->server_name_length == 6);
}

void test_serialize(){
    //void *ptr = reg_serialize();
}

void test_deserialize(){
//    char source[12];
//    FILE *fp = fopen("/home/dv16/dv16jjo/edu/DOD_OU2/Chat_Service/pdu_handler/data.pdu", "r");
//    if (fp != NULL) {
//        size_t newLen = fread(source, sizeof(char), 12, fp);
//        if ( ferror( fp ) != 0 ) {
//            fputs("Error reading file", stderr);
//        } else {
//            source[newLen++] = '\0'; /* Just to be safe. */
//        }
//        fclose(fp);
//    }
//
    char* pdu2 = calloc(10, 1);
    pdu2[0] = 0;
    pdu2[1] = 2;
    pdu2[2] = 57;
    pdu2[3] = 5;
    pdu2[4] = 'a';
    pdu2[5] = 'b';
    reg* r = reg_deserialize(&pdu2);
    assert(r !=  NULL);
    assert(r->tcp_port == port);
    assert(r->server_name_length == server_name_length);
    assert(strcmp((char*)r->server_name, server_name) == 0);
}
