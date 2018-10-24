#include <fcntl.h>
#include <errno.h>
#include "error_handling.h"


void perror_exit(char msg[]) {
    perror(msg);
    exit(11);
}

void* safe_calloc(size_t num_size, size_t size){
    void* ptr = calloc(num_size, size);
    if(ptr == NULL){
        perror_exit("Could not allocate memory.");
    }
    return ptr;
}

int fd_is_valid(int fd) {
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}