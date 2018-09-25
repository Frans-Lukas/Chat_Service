
#include <stdio.h>
#include "pdu_handler_server-nameserver.h"



reg* create_REG(char* server_name, int tcp_port){
    reg *pdu_reg = (reg*)calloc(1, sizeof(reg));
    pdu_reg->code = OP_REG;

    int server_name_len = (int) strlen(server_name);
    if(server_name_len > sizeof(uint8_t)) {
        fprintf(stderr, "server_name to long");
        return NULL;
    }

    pdu_reg->server_name_length = (uint8_t) server_name_len;
    if(tcp_port > sizeof(uint16_t)){
        fprintf(stderr, "port to big");
        return NULL;
    }
    pdu_reg->tcp_port = (uint16_t) tcp_port;
    pdu_reg->server_name = build_words(server_name, 4);

    return pdu_reg;
}


