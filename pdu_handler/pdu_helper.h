//
// Created by c16fld on 2018-09-24.
//

#ifndef CHAT_SERVICE_PDU_HELPER_H
#define CHAT_SERVICE_PDU_HELPER_H
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "../error_handling.h"

typedef uint8_t op_code;


uint32_t *build_words(char* input_string, int bytes);

uint16_t add_padding(int i);

uint8_t create_checksum(char *message);

uint16_t add_padding(int i);
uint8_t create_checksum(char *message);

#endif //CHAT_SERVICE_PDU_HELPER_H
