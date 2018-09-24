#include "error_handling.h"


void perror_exit(char msg[]) {
    perror(msg);
    exit(1);
}