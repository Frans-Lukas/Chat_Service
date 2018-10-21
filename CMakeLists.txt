cmake_minimum_required(VERSION 3.7.2)
project(Chat_Service C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(pdu_handler)
include_directories(pdu_handler/client-nameserver)
include_directories(pdu_handler/client-nameserver/test_client-nameserver)
include_directories(pdu_handler/client-server)
include_directories(pdu_handler/client-server/pdu_join_test)
include_directories(pdu_handler/client-server/pdu_mess_test)
include_directories(pdu_handler/client-server/pdu_participants_test)
include_directories(pdu_handler/client-server/pdu_pjoin_test)
include_directories(pdu_handler/client-server/pdu_pleave_test)
include_directories(pdu_handler/client-server/pdu_quit_test)
include_directories(pdu_handler/server-nameserver)
include_directories(pdu_handler/server-nameserver/test_server-nameserver)
include_directories(socket_handler)

file(GLOB_RECURSE pdu_handler
        "pdu_handler/*.c"
        "pdu_handler/*.h")

file(GLOB_RECURSE socket_handler
        "socket_handler/*.c"
        "socket_handler/*.h"
        )

add_executable(Server
        ${pdu_handler}
        ${socket_handler}
        server.c
        error_handling.c
        error_handling.h
        chat_service_server.c
        chat_service_server.h
        )

add_executable(Client
        ${pdu_handler}
        ${socket_handler}
        error_handling.c
        error_handling.h
        client.c
        chat_service_client.c
        chat_service_client.h)


find_package(Threads)
target_link_libraries(Client ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Server ${CMAKE_THREAD_LIBS_INIT})