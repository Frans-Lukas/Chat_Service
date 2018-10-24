#include <zconf.h>
#include <fcntl.h>
#include <pdu_handler/client-server/pdu_handler_client-server.h>
#include <pdu_handler/client-nameserver/pdu_handler_client-nameserver.h>
#include <pdu_handler/server-nameserver/pdu_handler_server-nameserver.h>
#include <tiff.h>
#include "pdu_helper.h"

uint32_t *build_words(char *input_string, int bytes, int string_len) {
    int number_of_words = get_num_words(string_len + 1, bytes);
    uint32_t *string_as_4byte = safe_calloc((size_t) number_of_words, sizeof(uint32_t));
    pdu_cpy_chars(string_as_4byte, input_string, 0, (size_t) string_len);
    return string_as_4byte;
}

int get_num_words(int string_len, int bytes) {
    if(string_len == 0){
        return 0;
    }
    return string_len < bytes ? 1 : (string_len / bytes) + (string_len % bytes > 0 ? 1 : 0);
}

uint16_t add_padding(void *start_pos, int num_bytes_to_pad) {
    for (int i = 0; i < num_bytes_to_pad; ++i) {
        ((char *) start_pos)[i] = '\0';
    }
}

int pdu_serialize(PDU *pdu, char **data_ptr) {
    int length = 0;
    switch (pdu->op) {
        case OP_JOIN:
            length = pdu_join_serialize(pdu, data_ptr);
            break;
        case OP_QUIT:
            length = pdu_quit_serialize(pdu, data_ptr);
            break;
        case OP_PJOIN:
            length = pdu_pjoin_serialize(pdu, data_ptr);
            break;
        case OP_PLEAVE:
            length = pdu_pleave_serialize(pdu, data_ptr);
            break;
        case OP_MESS:
            length = pdu_mess_serialize(pdu, data_ptr);
            break;
        case OP_PARTICIPANTS:
            length = pdu_participants_serialize(pdu, data_ptr);
            break;
        case OP_SLIST:
            length = pdu_s_list_serialize(pdu, data_ptr);
            break;
        case OP_GETLIST:
            length = pdu_get_list_serialize(pdu, data_ptr);
            break;
        case OP_REG:
            length = pdu_reg_serialize(pdu, data_ptr);
            break;
        case OP_ACK:
            length = pdu_ack_serialize(pdu, data_ptr);
            break;
        case OP_ALIVE:
            length = pdu_alive_serialize(pdu, data_ptr);
            break;
        case OP_NOTREG:
            length = pdu_not_reg_serialize(pdu, data_ptr);
            break;
        default:
            return -1;
    }
    return length;
}

PDU *pdu_deserialize_next(int fd) {
    uint8 opcode = 0;
    if(read(fd, &opcode, 1) < 0){
        return NULL;
    }
    switch (opcode) {
        case OP_JOIN:
            return (PDU *) pdu_join_deserialize(fd);
        case OP_QUIT:
            return (PDU *) pdu_quit_create();
        case OP_PJOIN:
            return (PDU *) pdu_pjoin_deserialize(fd);
        case OP_PLEAVE:
            return (PDU *) pdu_pleave_deserialize(fd);
        case OP_MESS:
            return (PDU *) pdu_mess_deserialize(fd);
        case OP_PARTICIPANTS:
            return (PDU *) pdu_participants_deserialize(fd);
        case OP_SLIST:
            return (PDU *) pdu_s_list_deserialize(fd);
        case OP_ACK:
            return (PDU *) pdu_ack_deserialize(fd);
        case OP_NOTREG:
            return (PDU *) pdu_not_reg_deserialize(fd);
        default:
            return NULL;
    }
}

bool data_is_divisible_by_four(char *data) {
    return strlen(data) % 4 == 0;
}

uint32_t *string_to_words(char *string, int size) {
    uint32_t *words = safe_calloc((size_t) get_num_words(size, 4), sizeof(uint32_t));
    for (int i = 0; i < size; ++i) {
        memcpy(&((char *) words)[i], &string[i], 1);
    }
    return words;
}

char *array_to_string(char *pString[], int list_length, int* length) {
    size_t size_to_allocate = 0;
    for (int j = 0; j < list_length; ++j) {
        size_to_allocate += strlen(pString[j]);
    }
    size_to_allocate += list_length;
    char *string = safe_calloc(sizeof(char), size_to_allocate + 1);
    *length = (int) size_to_allocate;
    size_t position_to_memcpy_to = 0;
    for (int i = 0; i < list_length; ++i) {
        memcpy(string + position_to_memcpy_to, pString[i], strlen(pString[i]));
        position_to_memcpy_to += strlen(pString[i]) + 1;
        memcpy(string + position_to_memcpy_to, "\0", 1);
    }

    return string;
}

char **string_to_string_array(char *string, int num_strings, char *dest[]) {
    char *current_pointer_position = string;
    for (int i = 0; i < num_strings; ++i) {
        dest[i] = current_pointer_position;
        while (*current_pointer_position != '\0') {
            current_pointer_position++;
        }
        current_pointer_position++;
    }
    return dest;
}

void pdu_cpy_chars(void *dest, void *src, int index, size_t num_bytes) {
    memcpy(dest, ((char *) src) + index, num_bytes);
}

void print_current_working_dirr() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}

int open_fd(char *path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror_exit("open()");
    }
    return fd;
}

void read_from_fd(int fd, void *destination, int size) {
    if(size == 0){
        return;
    }
    if (read(fd, destination, (size_t) size) < 0) {
        perror("could not read from fd in read_from_fd\n");
        return;
    }
}

