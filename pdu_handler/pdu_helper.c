#include "pdu_helper.h"

uint32_t *build_words(char* input_string, int bytes){
    int string_len = (int) strlen(input_string);
    int number_of_words = string_len < bytes ? 1 : (string_len/bytes) + (string_len % bytes > 0 ? 1 : 0);
    return memcpy(calloc((size_t) number_of_words, sizeof(uint32_t)), input_string, (size_t) string_len);
}

__uint16_t add_padding(int i) {
    return NULL;
}
__uint8_t create_checksum(char *message) {
    return NULL;
}
