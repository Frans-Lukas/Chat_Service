
#ifndef CHAT_SERVICE_PDU_HANDLER_SERVER_NAMESERVER_H
#define CHAT_SERVICE_PDU_HANDLER_SERVER_NAMESERVER_H

#include "../pdu_helper.h"
#include "../../error_handling.h"
#include <string.h>
#include <inttypes.h>

#define OP_REG 0
#define OP_ACK 1
#define OP_ALIVE 2
#define OP_NOTREG 100

typedef struct reg {
    PDU pdu;
    uint8_t server_name_length;
    uint16_t tcp_port;
    uint32_t *server_name;
} reg;

typedef struct alive {
    PDU pdu;
    uint8_t nr_of_clients;
    uint16_t id_number;
} alive;

typedef struct ack {
    PDU pdu;
    uint8_t pad;
    uint16_t id_number;
} ack;

typedef struct not_reg {
    PDU pdu;
    uint8_t pad;
    uint16_t id_number;
} not_reg;

uint32_t *build_words(char *input_string, int bytes);

int reg_serialize(reg *pdu, char** data);

reg *reg_deserialize(int fd);

int alive_serialize(alive *pdu, char** data);

alive *alive_deserialize(int fd);

int ack_serialize(ack *pdu, char** data);

ack *ack_deserialize(int fd);

int not_reg_serialize(not_reg *pdu, char** data);

not_reg *not_reg_deserialize(int fd);

#endif //CHAT_SERVICE_PDU_HANDLER_SERVER_NAMESERVER_H

