#include <zconf.h>
#include <netinet/in.h>
#include "pdu_handler_client-server.h"
#include "pdu_tests.h"


pdu_quit *pdu_quit_create() {
    pdu_quit *pdu = safe_calloc(1, sizeof(pdu_quit));
    pdu->op = OP_QUIT;
    return pdu;
}

pdu_quit *pdu_quit_deserialize(int fd) {
    pdu_quit *pdu_to_return = safe_calloc(1, sizeof(pdu_quit));
    pdu_to_return->op = OP_QUIT;
    return pdu_to_return;
}

int pdu_quit_serialize(PDU *join_pdu, char **data_to_send) {
    pdu_quit *pdu = (pdu_quit *) join_pdu;
    *data_to_send = safe_calloc(1, sizeof(pdu_quit));
    memcpy(*data_to_send, &pdu->op, 1);
    memcpy(*data_to_send + 1, &pdu->pad1, 1);
    memcpy(*data_to_send + 2, &pdu->pad2, 2);
    return sizeof(pdu_quit);
}

pdu_join *pdu_join_create(char *identity) {
    pdu_join *pdu = safe_calloc(1, sizeof(pdu_join));
    pdu->op = OP_JOIN;
    if (strlen(identity) > MAX_IDENTITY_LENGTH) {
        perror_exit("identity length is too long. Needs to be less than 255 characters");
    }
    pdu->identity_length = (uint8_t) strlen(identity);
    pdu->identity = build_words(identity, 4, pdu->identity_length);
}

int pdu_join_serialize(PDU *join_pdu, char **data_to_send) {
    pdu_join *pdu = (pdu_join *) join_pdu;
    int size_of_data = sizeof(pdu_join) + get_num_words(pdu->identity_length, 4) * 4;
    *data_to_send = safe_calloc(1, (size_t) size_of_data);
    *data_to_send[0] = OP_JOIN;
    pdu_cpy_chars(*data_to_send + 1, &pdu->identity_length, 0, 1);
    pdu_cpy_chars(*data_to_send + 4, pdu->identity, 0, pdu->identity_length);
    return size_of_data;
}

pdu_join *pdu_join_deserialize(int fd) {
    pdu_join *pdu_to_return = safe_calloc(1, sizeof(pdu_join));
    pdu_to_return->op = OP_JOIN;
    read_from_fd(fd, &pdu_to_return->identity_length, 1);
    read_from_fd(fd, &pdu_to_return->padding, 2);
    uint8_t length = pdu_to_return->identity_length;
    pdu_to_return->identity = safe_calloc(sizeof(uint32_t), (size_t) get_num_words(length, 4));
    read_from_fd(fd, pdu_to_return->identity, length);
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
    pdu_participants *pdu = safe_calloc(1, sizeof(pdu_participants));
    pdu->op = OP_PARTICIPANTS;
    pdu->num_identities = (uint8_t) num_participants;
    pdu->participant_names = build_participant_words(participants, num_participants);
    pdu->length = (uint16_t) get_size_of_participants(pdu->participant_names, pdu->num_identities);
}

int pdu_participants_serialize(PDU *pdu, char** data_to_send) {
    pdu_participants *pdu_partici = (pdu_participants *) pdu;
    htons(pdu_partici->length);
    int size = (sizeof(pdu_participants) + ( get_num_words(pdu_partici->length, 4)) * 4);
    *data_to_send = safe_calloc(sizeof(char), (size_t) size);
    pdu_cpy_chars(*data_to_send, pdu_partici, 0, 4);
    memcpy(*data_to_send + 4, pdu_partici->participant_names, (size_t) get_num_words(pdu_partici->length, 4) * 4);
    return size;
}

pdu_participants *pdu_participants_deserialize(int fd) {
    pdu_participants *pdu_to_return = safe_calloc(1, sizeof(pdu_participants));
    pdu_to_return->op = OP_PARTICIPANTS;
    read_from_fd(fd, &pdu_to_return->num_identities, 1);
    read_from_fd(fd, &pdu_to_return->length, 2);
    pdu_to_return->participant_names = safe_calloc(sizeof(uint32_t), (size_t) get_num_words(pdu_to_return->length, 4));
    read_from_fd(fd, pdu_to_return->participant_names, get_num_words(pdu_to_return->length, 4) * 4);
    ntohs(pdu_to_return->length);
    return pdu_to_return;
}

pdu_mess *pdu_mess_create(char *identity, char *message) {
    pdu_mess *pdu = safe_calloc(1, sizeof(pdu_mess));
    pdu->op = OP_MESS;
    pdu->identity_length = (uint8_t) strlen(identity);
    pdu->message_length = (uint16_t) strlen(message);
    pdu->timestamp = (uint32_t) time(NULL);
    pdu->message = build_words(message, 4, pdu->message_length);
    pdu->client_identity = build_words(identity, 4, pdu->identity_length);
    pdu->checksum = create_checksum(pdu);
    fprintf(stderr, "");
}

uint8_t create_checksum(pdu_mess *message){
    int checksum = 0;
    checksum += message->op;
    checksum += message->padding_op;
    checksum += message->identity_length;
    checksum += message->checksum;
    checksum += ((uint8_t*)&message->message_length)[0];
    checksum += ((uint8_t*)&message->message_length)[1];
    checksum += ((uint8_t*)&message->padding_message_length)[0];
    checksum += ((uint8_t*)&message->padding_message_length)[1];
    checksum += ((uint8_t*)&message->timestamp)[0];
    checksum += ((uint8_t*)&message->timestamp)[1];
    checksum += ((uint8_t*)&message->timestamp)[2];
    checksum += ((uint8_t*)&message->timestamp)[3];
    for (int i = 0; i < message->message_length; ++i) {
        checksum += message->message[i];
    }
    for (int j = 0; j < message->identity_length; ++j) {
        checksum += message->client_identity[j];
    }
    checksum = checksum % 255;
    return ~(uint8_t)checksum;
}

size_t pdu_mess_size(pdu_mess *mess) {
    return sizeof(pdu_mess) + get_num_words(mess->message_length, 4) * 4 + get_num_words(mess->identity_length, 4) * 4 -
           2 * sizeof(uint32_t);
}

int pdu_mess_serialize(PDU *pdu, char** data_to_send) {
    pdu_mess *pdu_message = (pdu_mess *) pdu;
    htons(pdu_message->message_length);
    htonl(pdu_message->timestamp);

    int size = (int) pdu_mess_size(pdu_message);
    *data_to_send = safe_calloc(sizeof(char), pdu_mess_size(pdu_message));
    pdu_cpy_chars(*data_to_send, pdu_message, 0, 12);
    pdu_cpy_chars(*data_to_send + 12, pdu_message->message, 0,
                  (size_t) get_num_words(pdu_message->message_length, 4) * 4);
    pdu_cpy_chars(*data_to_send + 12 + get_num_words(pdu_message->message_length, 4) * 4, pdu_message->client_identity,
                  0,
                  (size_t) get_num_words(pdu_message->identity_length, 4) * 4);
    return size;
}

pdu_mess *pdu_mess_deserialize(int fd) {
    pdu_mess *pdu_to_return = safe_calloc(1, sizeof(pdu_mess));
    pdu_to_return->op = OP_MESS;
    read_from_fd(fd, &pdu_to_return->padding_op, 1);
    read_from_fd(fd, &pdu_to_return->identity_length, 1);
    read_from_fd(fd, &pdu_to_return->checksum, 1);
    read_from_fd(fd, &pdu_to_return->message_length, 2);
    read_from_fd(fd, &pdu_to_return->padding_message_length, 2);
    read_from_fd(fd, &pdu_to_return->timestamp, 4);
    size_t message_size = (size_t) get_num_words(pdu_to_return->message_length, 4) * 4;
    pdu_to_return->message = safe_calloc(1, message_size);
    read_from_fd(fd, pdu_to_return->message, (int) message_size);
    size_t identity_size = (size_t) get_num_words(pdu_to_return->identity_length, 4) * 4;
    pdu_to_return->client_identity = safe_calloc(1, identity_size);
    read_from_fd(fd, pdu_to_return->client_identity, (int) identity_size);
    ntohs(pdu_to_return->message_length);
    ntohl(pdu_to_return->timestamp);
    return pdu_to_return;
}

bool pdu_mess_is_valid(PDU *pdu) {
    pdu_mess *real_pdu = (pdu_mess *) pdu;
    if (create_checksum(real_pdu) == 0) {
        return true;
    }
    return false;
}

//
pdu_pleave *pdu_pleave_create(char *identity) {
    pdu_pleave *pdu = safe_calloc(1, sizeof(pdu_pleave));
    pdu->op = OP_PLEAVE;
    pdu->identity_length = (uint8_t) strlen(identity);
    pdu->timestamp = (uint32_t) time(NULL);
    pdu->client_identity = build_words(identity, 4, pdu->identity_length);
}


pdu_pleave *pdu_pleave_deserialize(int fd) {
    pdu_pleave *pdu_to_return = safe_calloc(1, sizeof(pdu_pleave));
    pdu_to_return->op = OP_PLEAVE;
    read_from_fd(fd, &pdu_to_return->identity_length, 1);
    read_from_fd(fd, &pdu_to_return->padding_identity_length, 2);
    read_from_fd(fd, &pdu_to_return->timestamp, 4);
    pdu_to_return->client_identity = safe_calloc(1, pdu_to_return->identity_length);
    read_from_fd(fd, pdu_to_return->client_identity, pdu_to_return->identity_length);
    ntohl(pdu_to_return->timestamp);
    return pdu_to_return;
}

int pdu_pleave_serialize(PDU *pleave_data, char** data_to_send) {
    pdu_pleave *pdu = (pdu_pleave *) pleave_data;
    htonl(pdu->timestamp);
    int size = sizeof(pdu_pleave) + pdu->identity_length;
    *data_to_send = safe_calloc(sizeof(char), (size_t) size);
    pdu_cpy_chars(*data_to_send, pdu, 0, 8);
    pdu_cpy_chars(*data_to_send + 8, pdu->client_identity, 0, (size_t) get_num_words(pdu->identity_length, 4) * 4);

    return size;
}


pdu_pjoin *pdu_pjoin_create(char *identity) {
    pdu_pjoin *pdu = safe_calloc(1, sizeof(pdu_pjoin));
    pdu->op = OP_PJOIN;
    pdu->identity_length = (uint8_t) strlen(identity);
    pdu->timestamp = (uint32_t) time(NULL);
    pdu->client_identity = build_words(identity, 4, pdu->identity_length);
}

pdu_pjoin *pdu_pjoin_deserialize(int fd) {
    pdu_pjoin *pdu_to_return = safe_calloc(1, sizeof(pdu_pjoin));
    pdu_to_return->op = OP_PJOIN;
    read_from_fd(fd, &pdu_to_return->identity_length, 1);
    read_from_fd(fd, &pdu_to_return->padding_identity_length, 2);
    read_from_fd(fd, &pdu_to_return->timestamp, 4);
    pdu_to_return->client_identity = safe_calloc(1, pdu_to_return->identity_length);
    read_from_fd(fd, pdu_to_return->client_identity, pdu_to_return->identity_length);
    ntohl(pdu_to_return->timestamp);
    return pdu_to_return;
}

int pdu_pjoin_serialize(PDU *pjoin_data, char** data_to_send) {
    pdu_pjoin *pdu = (pdu_pjoin *) pjoin_data;
    htonl(pdu->timestamp);
    int size = sizeof(pdu_pjoin) + pdu->identity_length;
    *data_to_send = safe_calloc(sizeof(char), size);
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
    uint32_t *words = safe_calloc(sizeof(uint32_t), (size_t) get_num_words((int) size, 4) * 4);
    size_t pos = 0;
    for (int i = 0; i < num_participants; ++i) {
        memcpy(((char *) words) + pos, participants + pos, strlen(participants + pos) + 1);
        pos += strlen(participants + pos) + 1;
    }
    return words;
}