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

#define MAX_IDENTITY_LENGTH 255

typedef struct pdu_join{
    op_code op;
    __uint8_t identity_length;
    __uint16_t padding;
    __uint32_t identity;
} pdu_join;

typedef struct  pdu_participants{
    op_code op;
    __uint8_t  num_identities;
    __uint16_t length;
    __uint32_t participant_array[];
} pdu_participants;

typedef struct pdu_quit{
    op_code op;
} pdu_quit;

typedef struct pdu_mess{
    op_code op;
    __uint8_t padding_op;
    __uint8_t identity_length;
    __uint8_t checksum;
    __uint16_t message_length;
    __uint16_t padding_message_length;
    __uint32_t timestamp;
    __uint32_t message;
    __uint32_t client_identity;
} pdu_mess;

typedef struct pdu_pleave{
    op_code op;
    __uint8_t identity_length;
    __uint16_t padding_identity_length;
    __uint32_t timestamp;
    __uint32_t client_identity;

} pdu_pleave;

typedef struct pdu_pjoin{
    op_code op;
    __uint8_t identity_length;
    __uint16_t padding_identity_length;
    __uint32_t timestamp;
    __uint32_t client_identity;

} pdu_pjoin;

pdu_quit* pdu_quit_create();
pdu_join* pdu_join_create(char* identity);

#endif //CHAT_SERVICE_PDU_HANDLER_CLIENT_SERVER_H
