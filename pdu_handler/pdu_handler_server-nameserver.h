
#ifndef CHAT_SERVICE_PDU_HANDLER_SERVER_NAMESERVER_H
#define CHAT_SERVICE_PDU_HANDLER_SERVER_NAMESERVER_H
#include "pdu_helper.h"
#include "../error_handling.h"
#include <string.h>
#include <inttypes.h>
#define OP_REG 0
#define OP_ACK 1
#define OP_ALIVE 2
#define OP_NOTREG 100

typedef struct reg{
    PDU pdu;
    uint8_t server_name_length;
    uint16_t tcp_port;
    uint32_t* server_name;
} reg;
uint32_t *build_words(char* input_string, int bytes);
reg* create_REG(char* server_name, int server_name_length, int tcp_port);
reg* reg_deserialize(void *ptr);
#endif //CHAT_SERVICE_PDU_HANDLER_SERVER_NAMESERVER_H

