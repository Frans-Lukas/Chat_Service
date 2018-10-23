#include <socket_handler/client_list.h>
#include "pdu_particpants_tests.h"

void run_pdu_participants_tests() {
    assert_pdu_participants_create_works();
    assert_serialize_pdu_participants_works();
    assert_deserialize_pdu_participants_works();
    test_build_participant_words();
    test_client_list_participant_string();
}

void assert_pdu_participants_create_works() {
    char* names = "Anna\0Petter\0Lisa";
    pdu_participants* pdu = pdu_participants_create(names, 3, 16);
    char* participant_names = (char*) pdu->participant_names;
    assert(pdu->pdu.op == OP_PARTICIPANTS);
    assert(pdu->num_identities == 3);
    assert(strcmp(participant_names, "Anna") == 0);
    assert(strcmp(&participant_names[5], "Petter") == 0);
    assert(strcmp(&participant_names[12], "Lisa") == 0);
    free(pdu->participant_names);
    free(pdu);
}

void assert_serialize_pdu_participants_works() {
    char* string = "pe\0pe\0";
    char* real_serialized_pdu;
    pdu_participants* pdu = pdu_participants_create(string, 2, 6);
    htons(pdu->length);
    pdu_participants_serialize((PDU *) pdu, &real_serialized_pdu);
    assert(real_serialized_pdu[0] == OP_PARTICIPANTS);
    assert(real_serialized_pdu[1] == 2);
    assert(((uint16_t*)real_serialized_pdu)[1] == 6);
    assert(strncmp(real_serialized_pdu + 4, string, 2) == 0);
    assert(strncmp(real_serialized_pdu + 7, string, 2) == 0);
    free(real_serialized_pdu);
}

void assert_deserialize_pdu_participants_works() {
    char* string = "an\0pe";
    int fd = open_fd("../pdu_handler/client-server/pdu_participants_test/data.pdu");
    int op;
    read_from_fd(fd, &op, 1);
    pdu_participants* deserialized_pdu = pdu_participants_deserialize(fd);
    assert(deserialized_pdu->pdu.op == OP_PARTICIPANTS);
    assert(deserialized_pdu->num_identities == 2);
    htons(deserialized_pdu->length);
    assert(deserialized_pdu->length == 5);
    assert(strncmp((char*)deserialized_pdu->participant_names, string, deserialized_pdu->length) == 0);
    free(deserialized_pdu);

}


void test_build_participant_words(){
    char *kuba = "kubelito\0frasse\0";

    char *participants = (char *) build_participant_words(kuba, 2, 16);

    for(int i = 0; i < 16 ; i++){
        fprintf(stderr, "%c", (char)participants[i]);
    }
    assert(strcmp(participants, "kubelito") == 0);
    assert(strcmp(participants + 8, "frasse") == 0);
}

void test_client_list_participant_string(){
    client_list * cl = client_list_create();
    char *participants_list = calloc(2, sizeof(char*));

    client  kuba = {0};
    kuba.identity = "Kubelito";
    kuba.socket = 5;
    client frasse = {0};
    frasse.identity = "Frasselito";
    frasse.socket = 7;

    client_list_add_client(kuba, cl);
    client_list_add_client(frasse, cl);
    int* test = calloc(1, sizeof(int));
    int num_of_participants = client_list_create_participants_string(cl, &participants_list, test);
    assert(num_of_participants == 2);
}

void test_array_to_string(){
    char *pList[3];

    pList[0] = "Kuba1\0";
    pList[1] = "Kuba2\0";
    pList[2] = "Kuba3\0";


    //array_to_string();
}
