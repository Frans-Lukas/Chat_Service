//
// Created by c16fld on 2018-10-10.
//

#include <pdu_handler/pdu_helper.h>
#include <pdu_handler/client-server/pdu_handler_client-server.h>
#include <assert.h>
#include "socket_interface_test.h"


void socket_interface_test_all(){
    test_socket_interface_serialize_works();
    test_socket_interface_deserialize_works();
}

void test_socket_interface_deserialize_works(){
    char* string = "an\0pe";
    int fd = open_fd("../pdu_handler/client-server/pdu_participants_test/data.pdu");
    pdu_participants* deserialized_pdu = (pdu_participants *) pdu_deserialize_next(fd);
    assert(deserialized_pdu->op == OP_PARTICIPANTS);
    assert(deserialized_pdu->num_identities == 2);
    htons(deserialized_pdu->length);
    assert(deserialized_pdu->length == 5);
    assert(strncmp((char*)deserialized_pdu->participant_names, string, deserialized_pdu->length) == 0);
    free(deserialized_pdu);
}


void test_socket_interface_serialize_works(){
    char* string = "pe\0pe\0";
    char* real_serialized_pdu;
    pdu_participants* pdu = pdu_participants_create(string, 2);
    htons(pdu->length);
    pdu_serialize((PDU *) pdu, &real_serialized_pdu);
    assert(real_serialized_pdu[0] == OP_PARTICIPANTS);
    assert(real_serialized_pdu[1] == 2);
    assert(((uint16_t*)real_serialized_pdu)[1] == 6);
    assert(strncmp(real_serialized_pdu + 4, string, 2) == 0);
    assert(strncmp(real_serialized_pdu + 7, string, 2) == 0);
    free(real_serialized_pdu);
}