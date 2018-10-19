
#include <zconf.h>
#include <netinet/in.h>
#include "pdu_handler_client-nameserver.h"


//s_list* pdu_s_list_create(int number_of_servers, char adress[], int port[], int number_of_clients[], int server_name_length[]){
//    s_list* pdu = calloc(1, sizeof(s_list));
//    pdu->pdu.op = OP_SLIST;
//    pdu->number_of_servers = (uint16_t)number_of_servers;
//    pdu->adress = safe_calloc(pdu->number_of_servers, sizeof(uint32_t));
//    pdu->port = safe_calloc(pdu->number_of_servers, sizeof(uint16_t));
//    pdu->number_of_clients = safe_calloc(pdu->number_of_servers, sizeof(uint8_t));
//    pdu->server_name_length = safe_calloc(pdu->number_of_servers, sizeof(uint8_t));
//    pdu->server_name = safe_calloc(pdu->number_of_servers, sizeof(uint32_t));number_of_servers
//    for(int i = 0; i < number_of_servers; i++){
//        pdu->port[i] = (uint16_t) port[i];
//        pdu->number_of_clients[i] = (uint8_t) number_of_clients[i];
//    }
//}

int pdu_s_list_serialize(PDU *p, char **data) {
//    s_list* pdu = (s_list*)p;
//    int size = 12 + (get_num_words((int) pdu->server_name_length, 4) * 4);
//    *data = safe_calloc(1, sizeof(s_list));
//    *data[0] = OP_SLIST;
//    pdu->number_of_servers = htons(pdu->number_of_servers);
//    pdu_cpy_chars(*data + 2, &pdu->number_of_servers, 0, 2);
//    pdu_cpy_chars(*data + 4, &pdu->adress, 0, 4);
//    pdu->port = htons(pdu->port);
//    pdu_cpy_chars(*data + 8, &pdu->port, 0, 2);
//    pdu_cpy_chars(*data + 10, &pdu->number_of_clients, 0, 1);
//    pdu_cpy_chars(*data + 11, &pdu->server_name_length, 0, 1);
//    pdu_cpy_chars(*data + 12, &pdu->server_name, 0, (size_t) pdu->server_name_length);

    return 1;
}

s_list *pdu_s_list_deserialize(int fd) {
    s_list *pdu = safe_calloc(1, sizeof(s_list));
    pdu->pdu.op = OP_SLIST;
    read_from_fd(fd, &pdu->pad, 1);
    read_from_fd(fd, &pdu->number_of_servers, 2);
    pdu->number_of_servers = ntohs(pdu->number_of_servers);

    pdu->adress = safe_calloc(pdu->number_of_servers, sizeof(uint32_t));
    pdu->port = safe_calloc(pdu->number_of_servers, sizeof(uint16_t));
    pdu->number_of_clients = safe_calloc(pdu->number_of_servers, sizeof(uint8_t));
    pdu->server_name_length = safe_calloc(pdu->number_of_servers, sizeof(uint8_t));
    pdu->server_name = safe_calloc(pdu->number_of_servers, sizeof(uint32_t));

    for(int i=0; i<pdu->number_of_servers; i++){
        read_from_fd(fd, &pdu->adress[i], 4);
        read_from_fd(fd, &pdu->port[i], 2);
        pdu->port[i] = ntohs(pdu->port[i]);
        read_from_fd(fd, &pdu->number_of_clients[i], 1);
        read_from_fd(fd, &pdu->server_name_length[i], 1);
        pdu->server_name[i] = safe_calloc(pdu->server_name_length[i], sizeof(uint8_t));
        read_from_fd(fd, pdu->server_name[i], get_num_words(pdu->server_name_length[i], 4)*4);
    }
    return pdu;
}


get_list *pdu_create_get_list() {
    get_list* pdu = calloc(1, sizeof(get_list));
    pdu->pdu.op = OP_GETLIST;
    pdu->pad = calloc(3, sizeof(uint8_t));
    return pdu;
}

int pdu_get_list_serialize(PDU *p, char **data_to_send) {
    get_list* pdu = (get_list*)p;
    *data_to_send = safe_calloc(1, sizeof(uint32_t));
    *data_to_send[0] = pdu->pdu.op;
    return 4;
}

get_list *pdu_get_list_deserialize(int fd) {
    get_list *pdu = safe_calloc(1, sizeof(get_list));
    pdu->pad = safe_calloc(3, sizeof(uint8_t));
    pdu->pdu.op = OP_GETLIST;
    read_from_fd(fd, pdu->pad, 3);
    return pdu;
}


