
#ifndef CHAT_SERVICE_PDU_HELPER_H
#define CHAT_SERVICE_PDU_HELPER_H
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include "../error_handling.h"

typedef uint8_t op_code;

typedef struct PDU{
    op_code op;
} PDU;


uint32_t *build_words(char* input_string, int bytes);

uint16_t add_padding(int i);

uint8_t create_checksum(char *message);

uint16_t add_padding(int i);
uint8_t create_checksum(char *message);
char *array_to_string(char *pString[], int list_length);


#endif //CHAT_SERVICE_PDU_HELPER_H
