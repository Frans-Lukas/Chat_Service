#include "error_handling.h"


void perror_exit(char msg[]) {
    perror(msg);
    exit(1);
}

void* safe_calloc(size_t num_size, size_t size){
    void* ptr = calloc(num_size, size);
    if(ptr == NULL){
        perror_exit("Could not allocate memory.");
    }
    return ptr;
}