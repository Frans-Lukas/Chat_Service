#include "pdu_handler_client-server.h"


pdu_quit* pdu_quit_create(){
    pdu_quit* pdu = calloc(1, sizeof(pdu_quit));
    pdu->op = OP_QUIT;
    return pdu;
}

pdu_join* pdu_join_create(char* identity){
    pdu_join* pdu = calloc(1, sizeof(pdu_join));
    pdu->op = OP_JOIN;
    if(strlen(identity) > MAX_IDENTITY_LENGTH){
        perror("identity length is too long. Needs to be less than 255 characters");
        return NULL;
    }
    pdu->identity_length = (__uint8_t)strlen(identity);
    pdu->identity = string_to_uint32_array(identity);

}
