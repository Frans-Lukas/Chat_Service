#include "pdu_helper.h"

uint32_t *build_words(char* input_string, int bytes){
    int string_len = (int) strlen(input_string);
    int number_of_words = get_num_words(string_len, bytes);
    uint32_t* string_as_4byte = memcpy(calloc((size_t) number_of_words, sizeof(uint32_t)), input_string, (size_t) string_len);
    return string_as_4byte;
}

int get_num_words(int string_len, int bytes){
    return string_len < bytes ? 1 : (string_len/bytes) + (string_len % bytes > 0 ? 1 : 0);
}

uint16_t add_padding(void* start_pos, int num_bytes_to_pad) {
    for (int i = 0; i < num_bytes_to_pad; ++i) {
        ((char*)start_pos)[i] = '\0';
    }
}

__uint8_t create_checksum(char *message) {
    return 0x1;
}

bool data_is_divisible_by_four(char* data){
    return strlen(data) % 4 == 0;
}

uint32_t * string_to_words(char *string, int size){
    uint32_t* words = calloc((size_t) get_num_words(size, 4), sizeof(uint32_t));
    for (int i = 0; i < size; ++i) {
        memcpy(&((char*)words)[i], &string[i], 1);
    }
    return words;
}

char *array_to_string(char *pString[], int list_length) {
    size_t size_to_allocate = 0;
    for (int j = 0; j < list_length; ++j) {
        size_to_allocate += strlen(pString[j]) + 1;
    }
    size_to_allocate += list_length + 1;
    char* string = calloc(1, size_to_allocate);
    for (int i = 0; i < list_length; ++i) {
        strcat(string, pString[i]);
        strcat(string, "\0");
    }
    return string;
}

