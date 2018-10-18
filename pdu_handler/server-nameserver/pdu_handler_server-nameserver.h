
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

reg* pdu_create_reg(int server_name_length, int tcp_port, char *server_name);

alive* pdu_create_alive(int number_of_clients, int id_number);

not_reg* pdu_create_not_reg(int id_number);

ack* pdu_create_ack(int id_number);

int pdu_reg_serialize(PDU *p, char **data);

reg *pdu_reg_deserialize(int fd);

int pdu_alive_serialize(PDU *p, char **data);

alive *pdu_alive_deserialize(int fd);

int pdu_ack_serialize(PDU *p, char **data);

ack *pdu_ack_deserialize(int fd);

int pdu_not_reg_serialize(PDU *p, char **data);

not_reg *pdu_not_reg_deserialize(int fd);

#endif //CHAT_SERVICE_PDU_HANDLER_SERVER_NAMESERVER_H

