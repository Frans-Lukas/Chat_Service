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
    pdu->identity_length = (uint8_t)strlen(identity);
    pdu->padding = add_padding(2);
    pdu->identity = build_words(identity, 4);
}

pdu_join* pdu_join_deserialize(PDU* pdu_join){

}









pdu_participants* pdu_participants_create(char* participants[], int num_participants){
    pdu_participants* pdu = calloc(1, sizeof(pdu_participants));
    pdu->op = OP_PARTICIPANTS;
    pdu->num_identities = (uint8_t) num_participants;
    char* participants_string = array_to_string(participants, num_participants);
    pdu->length = (uint16_t) strlen(participants_string);
    pdu->participant_names = build_words(participants_string, 4);

}

pdu_mess* pdu_mess_create(char* identity, char* message){
    pdu_mess* pdu = calloc(1, sizeof(pdu_mess));
    pdu->op = OP_MESS;
    pdu->padding_op = (uint8_t) add_padding(1);
    pdu->identity_length = (uint8_t) strlen(identity);
    pdu->checksum = create_checksum(message);
    pdu->message_length = (uint16_t) strlen(message);
    pdu->timestamp = (uint32_t) time;
    pdu->message = build_words(message, 4);
    pdu->client_identity = build_words(identity, 4);
}

pdu_pleave* pdu_pleave_create(char* identity){
    pdu_pleave* pdu = calloc(1, sizeof(pdu_pleave));
    pdu->op = OP_PLEAVE;
    pdu->identity_length = (uint8_t) strlen(identity);
    pdu->padding_identity_length = add_padding(2);
    pdu->timestamp = (uint32_t) time;
    pdu->client_identity = build_words(identity, 4);
}

pdu_pjoin* pdu_pjoin_create(char* identity){
    pdu_pjoin* pdu = calloc(1, sizeof(pdu_pjoin));
    pdu->op = OP_PLEAVE;
    pdu->identity_length = (uint8_t) strlen(identity);
    pdu->padding_identity_length = add_padding(2);
    pdu->timestamp = (uint32_t) time;
    pdu->client_identity = build_words(identity, 4);
}