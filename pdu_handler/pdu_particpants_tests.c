#include "pdu_particpants_tests.h"

void run_pdu_participants_tests() {
    assert_pdu_participants_create_works();
    //assert_serialize_pdu_participants_works();
    //assert_deserialize_pdu_participants_works();
}

void assert_pdu_participants_create_works() {
    char* names[] = {
            "Anna",
            "Petter",
            "Lisa"
    };
    pdu_participants* pdu = pdu_participants_create(names, 3);
    assert(pdu->op == OP_PARTICIPANTS);
    assert(pdu->num_identities == 3);
    fprintf(stderr, "%s\n", (char*)(pdu->participant_names));
    assert(strcmp((char*)pdu->participant_names, "Anna") == 0);
    assert(strcmp((char*)(&pdu->participant_names[5]), "Petter") == 0);
    free(pdu);
}

void assert_serialize_pdu_participants_works() {
    char* string = "hello";
    char* mock_serialized_pdu = calloc(1, 1 + 1 + 2 + 8);
    mock_serialized_pdu[0] = OP_JOIN;
    mock_serialized_pdu[1] = 5;
    mock_serialized_pdu[2] = 0;
    mock_serialized_pdu[3] = 0;
    mock_serialized_pdu[4] = 'h';
    mock_serialized_pdu[5] = 'e';
    mock_serialized_pdu[6] = 'l';
    mock_serialized_pdu[7] = 'l';
    mock_serialized_pdu[8] = 'o';
    mock_serialized_pdu[9] = 'o';
    mock_serialized_pdu[10] = 'o';
    mock_serialized_pdu[11] = '\0';

    char* real_serialized_pdu = pdu_join_serialize((PDU *) pdu_join_create(string));
    //add same padding as mock pdu to make sure string compare works
    real_serialized_pdu[9] = 'o';
    real_serialized_pdu[10] = 'o';
    real_serialized_pdu[11] = '\0';
    assert(strcmp(real_serialized_pdu, mock_serialized_pdu) == 0);
    free(real_serialized_pdu);
    free(mock_serialized_pdu);
}

void assert_deserialize_pdu_participants_works() {
    char* string = "hello";
    char* mock_serialized_pdu = calloc(1, 1 + 1 + 2 + 8);
    mock_serialized_pdu[0] = OP_JOIN;
    mock_serialized_pdu[1] = 5;
    mock_serialized_pdu[2] = 0;
    mock_serialized_pdu[3] = 0;
    mock_serialized_pdu[4] = 'h';
    mock_serialized_pdu[5] = 'e';
    mock_serialized_pdu[6] = 'l';
    mock_serialized_pdu[7] = 'l';
    mock_serialized_pdu[8] = 'o';
    mock_serialized_pdu[9] = 'o';
    mock_serialized_pdu[10] = 'o';
    mock_serialized_pdu[11] = 'o';
    pdu_join* deserialized_pdu = pdu_join_deserialize(mock_serialized_pdu);
    fprintf(stderr, "%s\n", (char*)deserialized_pdu->identity);
    assert(strcmp((char*)deserialized_pdu->identity, string) == 0);
    free(mock_serialized_pdu);
    free(deserialized_pdu);

}

