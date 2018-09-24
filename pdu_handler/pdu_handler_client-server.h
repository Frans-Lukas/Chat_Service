//
// Created by c16fld on 2018-09-24.
//

#ifndef CHAT_SERVICE_PDU_HANDLER_CLIENT_SERVER_H
#define CHAT_SERVICE_PDU_HANDLER_CLIENT_SERVER_H
#include "pdu_helper.h"

#define OP_MESS 10
#define OP_QUIT 11
#define OP_JOIN 12
#define OP_PJOIN 16
#define OP_PLEAVE 17
#define OP_PARTICIPANTS 19

typedef struct join{
    op_code op;
    __uint8_t identity_length;
    __uint16_t padding;
    __uint32_t identity;
} join;

typedef struct  participants{
    op_code op;
    __uint8_t  num_identities;
    __uint16_t length;
    __uint32_t participant_array[];
} participants;

typedef struct quit{
    op_code op;
} quit;

typedef struct mess{
    op_code op;
    __uint8_t padding_op;
    __uint8_t identity_length;
    __uint8_t checksum;
    __uint16_t message_length;
    __uint16_t padding_message_length;
    __uint32_t timestamp;
    __uint32_t message;
    __uint32_t client_identity;
} mess;

typedef struct pleave{
    op_code op;
    __uint8_t identity_length;
    __uint16_t padding_identity_length;
    __uint32_t timestamp;
    __uint32_t client_identity;

} pleave;

typedef struct pjoin{
    op_code op;
    __uint8_t identity_length;
    __uint16_t padding_identity_length;
    __uint32_t timestamp;
    __uint32_t client_identity;

} pjoin;

#endif //CHAT_SERVICE_PDU_HANDLER_CLIENT_SERVER_H
