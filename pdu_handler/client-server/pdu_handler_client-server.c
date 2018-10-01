#include <zconf.h>
#include "pdu_handler_client-server.h"
#include "pdu_tests.h"


pdu_quit *pdu_quit_create() {
    pdu_quit *pdu = calloc(1, sizeof(pdu_quit));
    pdu->op = OP_QUIT;
    return pdu;
}

pdu_quit *pdu_quit_deserialize(int fd) {
    pdu_quit *pdu_to_return = calloc(1, sizeof(pdu_quit));
    pdu_to_return->op = OP_QUIT;
    return pdu_to_return;
}

int pdu_quit_serialize(PDU *join_pdu, char **data_to_send) {
    pdu_quit *pdu = (pdu_quit *) join_pdu;
    *data_to_send = calloc(1, sizeof(pdu_quit));
    memcpy(*data_to_send, &pdu->op, 1);
    return sizeof(pdu_quit);
}

pdu_join *pdu_join_create(char *identity) {
    pdu_join *pdu = calloc(1, sizeof(pdu_join));
    pdu->op = OP_JOIN;
    if (strlen(identity) > MAX_IDENTITY_LENGTH) {
        perror_exit("identity length is too long. Needs to be less than 255 characters");
    }
    pdu->identity_length = (uint8_t) strlen(identity);
    pdu->identity = build_words(identity, 4);
}

int pdu_join_serialize(PDU *join_pdu, char **data_to_send) {
    pdu_join *pdu = (pdu_join *) join_pdu;
    int size_of_data = sizeof(pdu_join) + get_num_words(pdu->identity_length, 4) * 4;
    *data_to_send = calloc(1, (size_t) size_of_data);
    *data_to_send[0] = OP_JOIN;
    pdu_cpy_chars(*data_to_send + 1, &pdu->identity_length, 0, 1);
    pdu_cpy_chars(*data_to_send + 4, pdu->identity, 0, pdu->identity_length);
    return size_of_data;
}

pdu_join *pdu_join_deserialize(int fd) {
    pdu_join *pdu_to_return = calloc(1, sizeof(pdu_join));
    pdu_to_return->op = OP_JOIN;
    read_from_fd(fd, &pdu_to_return->identity_length, 1);
    read_from_fd(fd, &pdu_to_return->padding, 2);
    uint8_t length = pdu_to_return->identity_length;
    char *identity = calloc(1, pdu_to_return->identity_length);
    read_from_fd(fd, identity, pdu_to_return->identity_length);
    pdu_to_return->identity = string_to_words(identity, length);
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

int pdu_participants_serialize(PDU *pdu, char** data_to_send) {
    pdu_participants *pdu_partici = (pdu_participants *) pdu;
    int size = (sizeof(pdu_participants) + ( get_num_words(pdu_partici->length, 4)) * 4);
    *data_to_send = calloc(sizeof(char), (size_t) size);
    pdu_cpy_chars(*data_to_send, pdu_partici, 0, 4);
    memcpy(*data_to_send + 4, pdu_partici->participant_names, (size_t) get_num_words(pdu_partici->length, 4) * 4);
    return size;
}

pdu_participants *pdu_participants_deserialize(int fd) {
    pdu_participants *pdu_to_return = calloc(1, sizeof(pdu_participants));
    pdu_to_return->op = OP_PARTICIPANTS;
    read_from_fd(fd, &pdu_to_return->num_identities, 1);
    read_from_fd(fd, &pdu_to_return->length, 2);
    pdu_to_return->participant_names = calloc(sizeof(uint32_t), (size_t) get_num_words(pdu_to_return->length, 4));
    read_from_fd(fd, pdu_to_return->participant_names, get_num_words(pdu_to_return->length, 4) * 4);
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
    return sizeof(pdu_mess) + get_num_words(mess->message_length, 4) * 4 + get_num_words(mess->identity_length, 4) * 4 -
           2 * sizeof(uint32_t);
}

int pdu_mess_serialize(PDU *pdu, char** data_to_send) {
    pdu_mess *pdu_message = (pdu_mess *) pdu;
    int size = (int) pdu_mess_size(pdu_message);
    *data_to_send = calloc(sizeof(char), pdu_mess_size(pdu_message));
    pdu_cpy_chars(*data_to_send, pdu_message, 0, 12);
    pdu_cpy_chars(*data_to_send + 12, pdu_message->message, 0,
                  (size_t) get_num_words(pdu_message->message_length, 4) * 4);
    pdu_cpy_chars(*data_to_send + 12 + get_num_words(pdu_message->message_length, 4) * 4, pdu_message->client_identity,
                  0,
                  (size_t) get_num_words(pdu_message->identity_length, 4) * 4);
    return size;
}

pdu_mess *pdu_mess_deserialize(int fd) {
    pdu_mess *pdu_to_return = calloc(1, sizeof(pdu_mess));
    pdu_to_return->op = OP_MESS;
    read_from_fd(fd, &pdu_to_return->padding_op, 1);
    read_from_fd(fd, &pdu_to_return->identity_length, 1);
    read_from_fd(fd, &pdu_to_return->checksum, 1);
    read_from_fd(fd, &pdu_to_return->message_length, 2);
    read_from_fd(fd, &pdu_to_return->padding_message_length, 2);
    read_from_fd(fd, &pdu_to_return->timestamp, 4);
    size_t message_size = (size_t) get_num_words(pdu_to_return->message_length, 4) * 4;
    pdu_to_return->message = calloc(1, message_size);
    read_from_fd(fd, pdu_to_return->message, (int) message_size);
    size_t identity_size = (size_t) get_num_words(pdu_to_return->identity_length, 4) * 4;
    pdu_to_return->client_identity = calloc(1, identity_size);
    read_from_fd(fd, pdu_to_return->client_identity, (int) identity_size);
    return pdu_to_return;
}

bool pdu_mess_is_valid(PDU *pdu) {
    pdu_mess *real_pdu = (pdu_mess *) pdu;
    if (create_checksum((char *) real_pdu->message) == real_pdu->checksum) {
        return true;
    }
    return false;
}

//
pdu_pleave *pdu_pleave_create(char *identity) {
    pdu_pleave *pdu = calloc(1, sizeof(pdu_pleave));
    pdu->op = OP_PLEAVE;
    pdu->identity_length = (uint8_t) strlen(identity);
    pdu->timestamp = (uint32_t) time;
    pdu->client_identity = build_words(identity, 4);
}


pdu_pleave *pdu_pleave_deserialize(int fd) {
    pdu_pleave *pdu_to_return = calloc(1, sizeof(pdu_pleave));
    pdu_to_return->op = OP_PLEAVE;
    read_from_fd(fd, &pdu_to_return->identity_length, 1);
    read_from_fd(fd, &pdu_to_return->padding_identity_length, 2);
    read_from_fd(fd, &pdu_to_return->timestamp, 4);
    pdu_to_return->client_identity = calloc(1, pdu_to_return->identity_length);
    read_from_fd(fd, pdu_to_return->client_identity, pdu_to_return->identity_length);
    return pdu_to_return;
}

int pdu_pleave_serialize(PDU *pleave_data, char** data_to_send) {
    pdu_pleave *pdu = (pdu_pleave *) pleave_data;
    int size = sizeof(pdu_pleave) + pdu->identity_length;
    *data_to_send = calloc(sizeof(char), size);
    pdu_cpy_chars(*data_to_send, pdu, 0, 8);
    pdu_cpy_chars(*data_to_send + 8, pdu->client_identity, 0, (size_t) get_num_words(pdu->identity_length, 4) * 4);
    return size;
}


pdu_pjoin *pdu_pjoin_create(char *identity) {
    pdu_pjoin *pdu = calloc(1, sizeof(pdu_pjoin));
    pdu->op = OP_PJOIN;
    pdu->identity_length = (uint8_t) strlen(identity);
    pdu->timestamp = (uint32_t) time;
    pdu->client_identity = build_words(identity, 4);
}

pdu_pjoin *pdu_pjoin_deserialize(int fd) {
    pdu_pjoin *pdu_to_return = calloc(1, sizeof(pdu_pjoin));
    pdu_to_return->op = OP_PJOIN;
    read_from_fd(fd, &pdu_to_return->identity_length, 1);
    read_from_fd(fd, &pdu_to_return->timestamp, 4);
    pdu_to_return->client_identity = calloc(1, pdu_to_return->identity_length);
    read_from_fd(fd, pdu_to_return->client_identity, pdu_to_return->identity_length);
    return pdu_to_return;
}

int pdu_pjoin_serialize(PDU *pjoin_data, char** data_to_send) {
    pdu_pjoin *pdu = (pdu_pjoin *) pjoin_data;
    int size = sizeof(pdu_pjoin) + pdu->identity_length;
    *data_to_send = calloc(sizeof(char), size);
    pdu_cpy_chars(*data_to_send, pdu, 0, 8);
    pdu_cpy_chars(*data_to_send + 8, pdu->client_identity, 0, (size_t) get_num_words(pdu->identity_length, 4) * 4);
    return size;
}

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