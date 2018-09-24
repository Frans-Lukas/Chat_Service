cmake_minimum_required(VERSION 3.10)
project(Chat_service C)

set(CMAKE_C_STANDARD 99)

add_executable(Chat_service main.c pdu_handler/pdu_handler_server-nameserver.c pdu_handler/pdu_handler_server-nameserver.h pdu_handler/pdu_handler_client-nameserver.c pdu_handler/pdu_handler_client-nameserver.h pdu_handler/pdu_handler_client-server.c pdu_handler/pdu_handler_client-server.h pdu_handler/pdu_helper.h error_handling.c error_handling.h)