cmake_minimum_required(VERSION 3.10)
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

add_executable(Chat_Service
        pdu_handler/client-nameserver/test_client-nameserver/test_pdu_handler_client-nameserver.c
        pdu_handler/client-nameserver/test_client-nameserver/test_pdu_handler_client-nameserver.h
        pdu_handler/client-nameserver/pdu_handler_client-nameserver.c
        pdu_handler/client-nameserver/pdu_handler_client-nameserver.h
        pdu_handler/client-server/pdu_join_test/pdu_join_tests.c
        pdu_handler/client-server/pdu_join_test/pdu_join_tests.h
        pdu_handler/client-server/pdu_mess_test/pdu_mess_tests.c
        pdu_handler/client-server/pdu_mess_test/pdu_mess_tests.h
        pdu_handler/client-server/pdu_participants_test/pdu_particpants_tests.c
        pdu_handler/client-server/pdu_participants_test/pdu_particpants_tests.h
        pdu_handler/client-server/pdu_pjoin_test/pdu_pjoin_tests.c
        pdu_handler/client-server/pdu_pjoin_test/pdu_pjoin_tests.h
        pdu_handler/client-server/pdu_pleave_test/pdu_pleave_tests.c
        pdu_handler/client-server/pdu_pleave_test/pdu_pleave_tests.h
        pdu_handler/client-server/pdu_quit_test/pdu_quit_tests.c
        pdu_handler/client-server/pdu_quit_test/pdu_quit_tests.h
        pdu_handler/client-server/pdu_handler_client-server.c
        pdu_handler/client-server/pdu_handler_client-server.h
        pdu_handler/server-nameserver/test_server-nameserver/test_pdu_handler_server-nameserver.c
        pdu_handler/server-nameserver/test_server-nameserver/test_pdu_handler_server-nameserver.h
        pdu_handler/server-nameserver/pdu_handler_server-nameserver.c
        pdu_handler/server-nameserver/pdu_handler_server-nameserver.h
        pdu_handler/pdu_helper.c
        pdu_handler/pdu_helper.h
        pdu_handler/pdu_tests.c
        pdu_handler/pdu_tests.h
        socket_handler/network_helper.c
        socket_handler/network_helper.h
        socket_handler/queue.c
        socket_handler/queue.h
        socket_handler/socket_helper.c
        socket_handler/socket_helper.h
        socket_handler/socket_interface.c
        socket_handler/socket_interface.h
        error_handling.c
        error_handling.h
        main.c socket_handler/socket_interface_test.c socket_handler/socket_interface_test.h socket_handler/client_list.c socket_handler/client_list.h)

find_package(Threads)
target_link_libraries(Chat_Service ${CMAKE_THREAD_LIBS_INIT})