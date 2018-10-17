
#ifndef RING_NETWORK_HELPER_H
#define RING_NETWORK_HELPER_H

#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include "../error_handling.h"
int network_getFQDN(char *fqdn, size_t n);
int network_hostname_to_ip(const char *hostname, char *ip);
#endif


