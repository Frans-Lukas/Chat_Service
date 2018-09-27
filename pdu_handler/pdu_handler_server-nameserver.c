
#include <stdio.h>
#include <netinet/in.h>
#include "pdu_handler_server-nameserver.h"



reg* create_REG(char* server_name,int server_name_length, int tcp_port){
    reg *pdu_reg = (reg*)calloc(1, sizeof(reg));
    pdu_reg->pdu.op = OP_REG;
    if(server_name_length > 255) {
        fprintf(stderr, "server_name to long");
        return NULL;
    }
    pdu_reg->server_name_length = (uint8_t) server_name_length;
    if(tcp_port > 65535){
        fprintf(stderr, "port to big");
        return NULL;
    }
    pdu_reg->tcp_port = (uint16_t) tcp_port;
    pdu_reg->server_name = build_words(server_name, 4);
    return pdu_reg;
}


void* reg_serialize(reg* pdu){
    
}


reg* reg_deserialize(void *ptr){
    uint8_t* tmp_ptr = ptr;
    reg *pdu_reg = (reg*)calloc(1, sizeof(reg));
    pdu_reg->pdu.op = OP_REG;
    memcpy(&pdu_reg->server_name_length, &tmp_ptr[1], sizeof(uint8_t));
    memcpy(&pdu_reg->tcp_port, &tmp_ptr[2], sizeof(uint16_t));
    memcpy(&pdu_reg->server_name, &tmp_ptr[4], pdu_reg->server_name_length);
    return pdu_reg;
}


