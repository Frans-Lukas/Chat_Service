//
// Created by c16fld on 2018-09-24.
//

#ifndef CHAT_SERVICE_ERROR_HANDLING_H
#define CHAT_SERVICE_ERROR_HANDLING_H
#include <stdio.h>
#include <stdlib.h>

void perror_exit(char msg[]);
void* safe_calloc(size_t num_size, size_t size);
int fd_is_valid(int fd);
#endif //CHAT_SERVICE_ERROR_HANDLING_H
