//
// Created by c16fld on 2018-09-24.
//

#ifndef CHAT_SERVICE_PDU_HANDLER_SERVER_NAMESERVER_H
#define CHAT_SERVICE_PDU_HANDLER_SERVER_NAMESERVER_H
#include "pdu_helper.h"
#include <string.h>
#define OP_REG 0
#define OP_ACK 1
#define OP_ALIVE 2
#define OP_NOTREG 100


typedef struct reg{
    __uint8_t code;
    __uint8_t server_name_length;
    __uint16_t tcp_port;
    __uint32_t server_name [];
} reg;

#endif //CHAT_SERVICE_PDU_HANDLER_SERVER_NAMESERVER_H
