#include "pdu_handler_client-server.h"
#include "pdu_tests.h"


pdu_quit *pdu_quit_create() {
    pdu_quit *pdu = calloc(1, sizeof(pdu_quit));
    pdu->op = OP_QUIT;
    return pdu;
}

pdu_quit *pdu_quit_deserialize(void *quit_pdu) {
    pdu_quit *pdu_to_return = calloc(1, sizeof(pdu_quit));
    pdu_to_return->op = ((uint8_t *) quit_pdu)[0];
    return pdu_to_return;
}

void *pdu_quit_serialize(PDU *join_pdu) {
    pdu_quit *pdu = (pdu_quit *) join_pdu;
    char *data_to_send = calloc(1, 1);
    memcpy(data_to_send, &pdu->op, 1);
    return data_to_send;
}

pdu_join *pdu_join_create(char *identity) {
    pdu_join *pdu = calloc(1, sizeof(pdu_join));
    pdu->op = OP_JOIN;
    if (strlen(identity) > MAX_IDENTITY_LENGTH) {
        perror("identity length is too long. Needs to be less than 255 characters");
        return NULL;
    }
    pdu->identity_length = (uint8_t) strlen(identity);
    pdu->identity = build_words(identity, 4);
}

void *pdu_join_serialize(PDU *join_pdu) {
    pdu_join *pdu = (pdu_join *) join_pdu;
    char *data_to_send = calloc(1, (size_t) (4 + pdu->identity_length + ((4 - (pdu->identity_length % 4)) % 4)));
    memcpy(data_to_send, &pdu->op, 1);
    memcpy(data_to_send + 1, &pdu->identity_length, 1);
    add_padding(data_to_send + 2, 2);
    memcpy(data_to_send + 4, pdu->identity, (size_t) pdu->identity_length + ((4 - (pdu->identity_length % 4)) % 4));
    return data_to_send;
}


pdu_join *pdu_join_deserialize(void *join_pdu) {
    uint8_t *pdu = join_pdu;
    pdu_join *pdu_to_return = calloc(1, sizeof(pdu_join));
    pdu_to_return->op = OP_JOIN;
    pdu_to_return->identity_length = pdu[1];
    uint8_t length = pdu[1];
    pdu_to_return->identity = string_to_words((char *) &pdu[4], length);
    return pdu_to_return;
}

/**
 *
 * @param participants a string of names split by null.
 * @param num_participants number of names in the  string splitted by null.
 * example string participants = "Anna\0Petter\0Lisa" num_participants = 3
 * @return
 */
pdu_participants *pdu_participants_create(char *participants, int num_participants) {
    pdu_participants *pdu = calloc(1, sizeof(pdu_participants));
    pdu->op = OP_PARTICIPANTS;
    pdu->num_identities = (uint8_t) num_participants;
    pdu->participant_names = build_participant_words(participants, num_participants);
    pdu->length = (uint16_t) get_size_of_participants(pdu->participant_names, pdu->num_identities);
}

void *pdu_participants_serialize(PDU *pdu) {
    pdu_participants *pdu_partici = (pdu_participants *) pdu;
    char *data_to_send = calloc(sizeof(char), (1 + 1 + 2 + ((size_t) get_num_words(pdu_partici->length, 4)) * 4));
    pdu_cpy_chars(data_to_send, pdu_partici, 0, 4);
    memcpy(data_to_send + 4, pdu_partici->participant_names, (size_t) get_num_words(pdu_partici->length, 4) * 4);
    return data_to_send;
}

pdu_participants *pdu_participants_deserialize(void *participants_data) {
    uint8_t *pdu = participants_data;
    pdu_participants *pdu_to_return = calloc(1, sizeof(pdu_participants));
    pdu_to_return->op = OP_PARTICIPANTS;
    pdu_cpy_chars(&pdu_to_return->num_identities, pdu, 1, 1);
    pdu_cpy_chars(&pdu_to_return->length, pdu, 2, 2);
    pdu_to_return->participant_names = calloc(sizeof(uint32_t), (size_t) get_num_words(pdu_to_return->length, 4));
    pdu_cpy_chars(pdu_to_return->participant_names, pdu, 4,
                  (size_t) get_num_words(pdu_to_return->length, 4) * 4);
    return pdu_to_return;
}

pdu_mess *pdu_mess_create(char *identity, char *message) {
    pdu_mess *pdu = calloc(1, sizeof(pdu_mess));
    pdu->op = OP_MESS;
    pdu->identity_length = (uint8_t) strlen(identity);
    pdu->checksum = create_checksum(message);
    pdu->message_length = (uint16_t) strlen(message);
    pdu->timestamp = (uint32_t) time;
    pdu->message = build_words(message, 4);
    pdu->client_identity = build_words(identity, 4);
}

size_t pdu_mess_size(pdu_mess *mess) {
    return sizeof(pdu_mess) + get_num_words(mess->message_length, 4) * 4 + get_num_words(mess->identity_length, 4) * 4 - 2 * sizeof(uint32_t);
}

void *pdu_mess_serialize(PDU *pdu) {
    pdu_mess *pdu_message = (pdu_mess *) pdu;
    char *data_to_send = calloc(sizeof(char), pdu_mess_size(pdu_message));
    pdu_cpy_chars(data_to_send, pdu_message, 0, 12);
    pdu_cpy_chars(data_to_send + 12, pdu_message->message, 0,
                  (size_t) get_num_words(pdu_message->message_length, 4) * 4);
    pdu_cpy_chars(data_to_send + 12 + get_num_words(pdu_message->message_length, 4) * 4, pdu_message->client_identity, 0,
                  (size_t) get_num_words(pdu_message->identity_length, 4) * 4);
    return data_to_send;
}

//pdu_mess* pdu_mess_deserialize(void* mess_data){
//    uint8_t *pdu = mess_data;
//    pdu_mess *pdu_to_return = calloc(1, sizeof(pdu_mess));
//    pdu_to_return->op = OP_MESS;
//    pdu_cpy_chars(&pdu_to_return->num_identities, pdu, 1, 1);
//    pdu_cpy_chars(&pdu_to_return->length, pdu, 2, 2);
//    pdu_to_return->participant_names = calloc(sizeof(uint32_t), (size_t) get_num_words(pdu_to_return->length, 4));
//    pdu_cpy_chars(pdu_to_return->participant_names, pdu, 4,
//                  (size_t) get_num_words(pdu_to_return->length, 4) * 4);
//    return pdu_to_return;
//}

bool pdu_mess_validate(PDU *pdu) {
    pdu_mess *real_pdu = (pdu_mess *) pdu;
    if (create_checksum((char *) real_pdu->message) == real_pdu->checksum) {
        return true;
    }
    return false;
}

//
//pdu_pleave* pdu_pleave_create(char* identity){
//    pdu_pleave* pdu = calloc(1, sizeof(pdu_pleave));
//    pdu->op = OP_PLEAVE;
//    pdu->identity_length = (uint8_t) strlen(identity);
//    pdu->padding_identity_length = add_padding(2);
//    pdu->timestamp = (uint32_t) time;
//    pdu->client_identity = build_words(identity, 4);
//}
//
//pdu_pjoin* pdu_pjoin_create(char* identity){
//    pdu_pjoin* pdu = calloc(1, sizeof(pdu_pjoin));
//    pdu->op = OP_PLEAVE;
//    pdu->identity_length = (uint8_t) strlen(identity);
//    pdu->padding_identity_length = add_padding(2);
//    pdu->timestamp = (uint32_t) time;
//    pdu->client_identity = build_words(identity, 4);
//}

size_t get_size_of_participants(uint32_t *participants, uint8_t num_participants) {
    size_t size = 0;
    char *names = (char *) participants;
    for (int i = 0; i < num_participants; ++i) {
        size += strlen(names + size) + 1;
    }
    return size;
}

uint32_t *build_participant_words(char *participants, int num_participants) {
    size_t size = 0;
    char *currpos = participants;
    for (int i = 0; i < num_participants; ++i) {
        size += strlen(currpos);
        currpos += size + 1;
    }
    uint32_t *words = calloc(sizeof(uint32_t), (size_t) get_num_words((int) size, 4));
    size_t pos = 0;
    for (int i = 0; i < num_participants; ++i) {
        memcpy(((char *) words) + pos, participants + pos, strlen(participants + pos) + 1);
        pos += strlen(participants + pos) + 1;
    }
    return words;
}