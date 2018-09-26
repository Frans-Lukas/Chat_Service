#include "pdu_helper.h"

uint32_t *build_words(char* input_string, int bytes){
    int string_len = (int) strlen(input_string);
    int number_of_words = string_len < bytes ? 1 : (string_len/bytes) + (string_len % bytes > 0 ? 1 : 0);
    uint32_t* string_as_4byte = memcpy(calloc((size_t) number_of_words, sizeof(uint32_t)), input_string, (size_t) string_len);
    for (int i = 0; i < number_of_words; ++i) {
        htobe32(string_as_4byte[i]);
    }
    return string_as_4byte;
}

__uint16_t add_padding(int i) {
    return NULL;
}
__uint8_t create_checksum(char *message) {
    return NULL;
}

char *array_to_string(char *pString[], int list_length) {
    size_t size_to_allocate = 0;
    for (int j = 0; j < list_length; ++j) {
        size_to_allocate += strlen(pString[j]);
    }
    size_to_allocate += list_length + 1;
    char* string = calloc(1, size_to_allocate);
    for (int i = 0; i < list_length; ++i) {
        strcat(string,pString[i]);
    }
    return NULL;
}