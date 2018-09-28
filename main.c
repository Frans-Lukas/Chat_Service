#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pdu_handler/server-nameserver/pdu_handler_server-nameserver.h"
#include "pdu_handler/pdu_helper.h"
#include "pdu_handler/server-nameserver/test_pdu_handler_server-nameserver.h"
#include "pdu_handler/pdu_tests.h"

int main() {
//    char *test_string = "abcdefgh";
//    uint32_t *test_word = build_words(test_string, 4);
//    printf("%s" "\n", (char*)&test_word[0]);
//    printf("%s" "\n",(char*)&test_word[1]);
//    //test_creating_reg();
    run_all_tests();
    return 0;
}