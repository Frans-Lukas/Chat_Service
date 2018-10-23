

#include <pdu_handler/pdu_helper.h>
#include <pdu_handler/client-server/pdu_handler_client-server.h>
#include <assert.h>
#include <pthread.h>
#include "socket_interface_test.h"
#include "socket_interface.h"
#include "socket_helper.h"
#include "network_helper.h"


void socket_interface_test_all(){
    test_socket_interface_serialize_works();
    test_socket_interface_deserialize_works();

    pthread_t writer;

    pthread_create(&writer, NULL, &test_socket_pdu_read_tcp, NULL);
    test_socket_pdu_write_tcp(NULL);
    pthread_join(writer, NULL);
}

void* test_socket_pdu_read_tcp(void *data){
    int socket_to_read_from = start_test_client();
    PDU** pdu = NULL;
    while(pdu == NULL){
        pdu = socket_read_pdu_from(&socket_to_read_from, 1);
    }
    pdu_join* real_pdu = (pdu_join *) pdu[0];
    assert(real_pdu->pdu.op == OP_JOIN);
    assert(strncmp((char*)real_pdu->identity, "Kubadoo", real_pdu->identity_length) == 0);
}

void* test_socket_pdu_write_tcp(void *data){
    int socket_to_write_to = start_test_server();
    pdu_join* pdu = pdu_join_create("Kubadoo");
    while(socket_write_pdu_to((PDU *) pdu, &socket_to_write_to, 1) == -1);
}

int start_test_client(){
    pdu_join* pdu = pdu_join_create("Kubadoo");
    int socket_to_write_to = socket_tcp_create();

    char* hostname = safe_calloc(1, 256);
    char* ip = safe_calloc(1, 17);
    network_getFQDN(hostname, 256);
    network_hostname_to_ip(hostname, ip);
    socket_connect(port, ip, socket_to_write_to);

    free(ip);
    free(hostname);
    return socket_to_write_to;
}

int start_test_server(){
    int socket_to_read_from = socket_tcp_create();
    socket_bind(port, socket_to_read_from);
    socket_tcp_listen(socket_to_read_from);
    return socket_tcp_get_connecting_socket_by_accepting(socket_to_read_from);
}

void test_socket_interface_deserialize_works(){
    char* string = "an\0pe";
    int fd = open_fd("../pdu_handler/client-server/pdu_participants_test/data.pdu");
    pdu_participants* deserialized_pdu = (pdu_participants *) pdu_deserialize_next(fd);
    assert(deserialized_pdu->pdu.op == OP_PARTICIPANTS);
    assert(deserialized_pdu->num_identities == 2);
    htons(deserialized_pdu->length);
    assert(deserialized_pdu->length == 5);
    assert(strncmp((char*)deserialized_pdu->participant_names, string, deserialized_pdu->length) == 0);
    free(deserialized_pdu);
}

void test_socket_interface_serialize_works(){
    char* string = "pe\0pe\0";
    char* real_serialized_pdu;
    pdu_participants* pdu = pdu_participants_create(string, 2, 6);
    htons(pdu->length);
    pdu_serialize((PDU *) pdu, &real_serialized_pdu);
    assert(real_serialized_pdu[0] == OP_PARTICIPANTS);
    assert(real_serialized_pdu[1] == 2);
    assert(((uint16_t*)real_serialized_pdu)[1] == 6);
    assert(strncmp(real_serialized_pdu + 4, string, 2) == 0);
    assert(strncmp(real_serialized_pdu + 7, string, 2) == 0);
    free(real_serialized_pdu);
}

