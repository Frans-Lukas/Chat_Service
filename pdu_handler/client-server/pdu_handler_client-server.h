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
    uint8_t identity_length;
    uint16_t padding;
    uint32_t* identity;
} pdu_join;

typedef struct  pdu_participants{
    op_code op;
    uint8_t  num_identities;
    uint16_t length;
    uint32_t* participant_names;
} pdu_participants;

typedef struct pdu_quit{
    op_code op;
} pdu_quit;

typedef struct pdu_mess{
    op_code op;
    uint8_t padding_op;
    uint8_t identity_length;
    uint8_t checksum;
    uint16_t message_length;
    uint16_t padding_message_length;
    uint32_t timestamp;
    uint32_t* message;
    uint32_t* client_identity;
} pdu_mess;

typedef struct pdu_pleave{
    op_code op;
    uint8_t identity_length;
    uint16_t padding_identity_length;
    uint32_t timestamp;
    uint32_t* client_identity;

} pdu_pleave;

typedef struct pdu_pjoin{
    op_code op;
    uint8_t identity_length;
    uint16_t padding_identity_length;
    uint32_t timestamp;
    uint32_t* client_identity;

} pdu_pjoin;

pdu_quit* pdu_quit_create();


void *pdu_mess_serialize(PDU *pdu);
pdu_mess* pdu_mess_deserialize(void* mess_data);
pdu_quit* pdu_quit_deserialize(void* quit_pdu);
void* pdu_quit_serialize(PDU* join_pdu);
pdu_join* pdu_join_create(char* identity);
pdu_mess* pdu_mess_create(char* identity, char* message);
pdu_participants* pdu_participants_create(char* participants, int num_participants);
pdu_pleave* pdu_pleave_create(char* identity);
pdu_pjoin* pdu_pjoin_create(char* identity);
pdu_join* pdu_join_deserialize(void* join_pdu);
void* pdu_join_serialize(PDU* join_pdu);
bool pdu_join_is_valid(pdu_join* join);
uint32_t *build_participant_words(char *participants, int num_participants);
size_t get_size_of_participants(uint32_t *participants, uint8_t num_participants);
void *pdu_participants_serialize(PDU *pdu);
pdu_participants *pdu_participants_deserialize(void *participants_data);
void *pdu_pleave_serialize(PDU *pleave_data);
pdu_pleave *pdu_pleave_deserialize(void *pleave_data);
#endif //CHAT_SERVICE_PDU_HANDLER_CLIENT_SERVER_H
