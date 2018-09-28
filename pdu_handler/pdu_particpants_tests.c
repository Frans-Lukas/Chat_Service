#include "pdu_particpants_tests.h"

void run_pdu_participants_tests() {
    assert_pdu_participants_create_works();
    assert_serialize_pdu_participants_works();
    assert_deserialize_pdu_participants_works();
    //assert_serialize_pdu_participants_works();
    //assert_deserialize_pdu_participants_works();
}

void assert_pdu_participants_create_works() {
    char* names = "Anna\0Petter\0Lisa";
    pdu_participants* pdu = pdu_participants_create(names, 3);
    char* participant_names = (char*) pdu->participant_names;
    assert(pdu->op == OP_PARTICIPANTS);
    assert(pdu->num_identities == 3);
    assert(strcmp(participant_names, "Anna") == 0);
    assert(strcmp(&participant_names[5], "Petter") == 0);
    assert(strcmp(&participant_names[12], "Lisa") == 0);
    free(pdu->participant_names);
    free(pdu);
}

void assert_serialize_pdu_participants_works() {
    char* string = "pe\0pe\0";
    char* real_serialized_pdu = pdu_particiapants_serialize((PDU*)pdu_participants_create(string, 2));
    assert(real_serialized_pdu[0] == OP_PARTICIPANTS);
    assert(real_serialized_pdu[1] == 2);
    assert(((uint16_t*)real_serialized_pdu)[1] == 6);
    fprintf(stderr, "%s\n", real_serialized_pdu+4);
    assert(strncmp(real_serialized_pdu + 4, string, 2) == 0);
    assert(strncmp(real_serialized_pdu + 7, string, 2) == 0);
}

void assert_deserialize_pdu_participants_works() {
    char* string = "Anna\0Petter";
    char* mock_serialized_pdu = calloc(1, 1 + 1 + 2 + 12);
    mock_serialized_pdu[0] = OP_PARTICIPANTS;
    mock_serialized_pdu[1] = 2;
    ((uint16_t*)mock_serialized_pdu)[1] = 12;
    mock_serialized_pdu[4] = 'A';
    mock_serialized_pdu[5] = 'n';
    mock_serialized_pdu[6] = 'n';
    mock_serialized_pdu[7] = 'a';
    mock_serialized_pdu[8] = '\0';
    mock_serialized_pdu[9] = 'P';
    mock_serialized_pdu[10] = 'e';
    mock_serialized_pdu[11] = 't';
    mock_serialized_pdu[12] = 't';
    mock_serialized_pdu[13] = 'e';
    mock_serialized_pdu[14] = 'r';
    mock_serialized_pdu[15] = '\0';
    pdu_participants* deserialized_pdu = pdu_participants_deserialize(mock_serialized_pdu);
    fprintf(stderr, "%s\n", (char*)deserialized_pdu->participant_names);
    assert(deserialized_pdu->op == OP_PARTICIPANTS);
    assert(deserialized_pdu->num_identities == 2);
    assert(deserialized_pdu->length == 12);
    assert(strcmp((char*)deserialized_pdu->participant_names, string) == 0);
    free(mock_serialized_pdu);
    free(deserialized_pdu);

}

