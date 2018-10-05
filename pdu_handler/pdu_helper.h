
#ifndef CHAT_SERVICE_PDU_HELPER_H
#define CHAT_SERVICE_PDU_HELPER_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <netinet/in.h>
#include <time.h>
#include <stdbool.h>
#include "../error_handling.h"

typedef uint8_t op_code;

typedef struct PDU {
    op_code op;
} PDU;


uint32_t *build_words(char *input_string, int bytes);

int get_num_words(int string_len, int bytes);

uint16_t add_padding(void *, int);

int pdu_serialize(PDU* pdu, char** data_ptr);

PDU* pdu_deserialize_next(int fd);

char *array_to_string(char *pString[], int list_length);

bool data_is_divisible_by_four(char *data);

char **string_to_string_array(char *, int, char *[]);

uint32_t *string_to_words(char *string, int size);

void pdu_cpy_chars(void *dest, void *src, int index, size_t num_bytes);

void print_current_working_dirr();

int open_fd(char *path);

void read_from_fd(int fd, void *destination, int size);

#endif //CHAT_SERVICE_PDU_HELPER_H
