//void
// Created by c16fld on 2018-10-10.
//

#ifndef CHAT_SERVICE_SOCKET_INTERFACE_TEST_H
#define CHAT_SERVICE_SOCKET_INTERFACE_TEST_H
void socket_interface_test_all();
void test_socket_interface_deserialize_works();
void test_socket_interface_serialize_works();
int start_test_client();
int start_test_server();
void * test_socket_pdu_read(void* data);
void * test_socket_pdu_write(void* data);
#endif //CHAT_SERVICE_SOCKET_INTERFACE_TEST_H
