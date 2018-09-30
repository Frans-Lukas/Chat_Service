#include <zconf.h>
#include <fcntl.h>
#include "pdu_helper.h"

uint32_t *build_words(char* input_string, int bytes){
    int string_len = (int) strlen(input_string);
    int number_of_words = get_num_words(string_len, bytes);
    uint32_t* string_as_4byte = memcpy(calloc((size_t) number_of_words, sizeof(uint32_t)), input_string, (size_t) string_len);
    return string_as_4byte;
}

int get_num_words(int string_len, int bytes){
    return string_len < bytes ? 1 : (string_len/bytes) + (string_len % bytes > 0 ? 1 : 0);
}

uint16_t add_padding(void* start_pos, int num_bytes_to_pad) {
    for (int i = 0; i < num_bytes_to_pad; ++i) {
        ((char*)start_pos)[i] = '\0';
    }
}

__uint8_t create_checksum(char *message) {
    return 0x1f;
}

bool data_is_divisible_by_four(char* data){
    return strlen(data) % 4 == 0;
}

uint32_t * string_to_words(char *string, int size){
    uint32_t* words = calloc((size_t) get_num_words(size, 4), sizeof(uint32_t));
    for (int i = 0; i < size; ++i) {
        memcpy(&((char*)words)[i], &string[i], 1);
    }
    return words;
}

char *array_to_string(char *pString[], int list_length) {
    size_t size_to_allocate = 0;
    for (int j = 0; j < list_length; ++j) {
        size_to_allocate += strlen(pString[j]);
    }
    size_to_allocate += list_length + 1;
    char* string = calloc(sizeof(char), size_to_allocate);

    size_t position_to_memcpy_to = 0;
    for (int i = 0; i < list_length; ++i) {
        memcpy(string + position_to_memcpy_to, pString[i], strlen(pString[i]) + 1);
        position_to_memcpy_to += strlen(pString[i]) + 1;
    }

    return string;
}

char** string_to_string_array(char *string, int num_strings, char* dest[]) {
    char* current_pointer_position = string;
    for (int i = 0; i < num_strings; ++i) {
        dest[i] = current_pointer_position;
        while(*current_pointer_position != '\0'){
            current_pointer_position++;
        }
        current_pointer_position++;
    }
    return dest;
}

void pdu_cpy_chars(void *dest, void *src, int index, size_t num_bytes) {
    memcpy(dest, ((char*)src) + index, num_bytes);
}

void print_current_working_dirr(){
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}

int open_fd(char *path){
    int fd = open("../pdu_handler/client-server/pdu_quit_test/data.pdu", O_RDONLY);
    if(fd < 0){
        perror_exit("open()");
    }
    return fd;
}

void read_from_fd(int fd, void *destination, int size) {
    if(read(fd, destination, size) < 0){
        perror_exit("read()");
    }
}