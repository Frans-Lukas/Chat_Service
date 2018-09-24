#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pdu_handler/pdu_handler_server-nameserver.h"

int main() {
    char *test_string = "abcdefgh";
    uint32_t *test_word = build_words(test_string, 4);
    printf("%s" "\n", (char*)&test_word[0]);
    printf("%s" "\n",(char*)&test_word[1]);
    return 0;
}