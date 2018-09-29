cmake_minimum_required(VERSION 3.10)
project(Chat_Service)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(pdu_handler)
include_directories(pdu_handler/client-nameserver)
include_directories(pdu_handler/server-nameserver)

add_executable(Chat_Service
        cmake-build-debug/Chat_service
        cmake-build-debug/Chat_service.cbp
        cmake-build-debug/cmake_install.cmake
        cmake-build-debug/CMakeCache.txt
        cmake-build-debug/Makefile
        pdu_handler/client-nameserver/pdu_handler_client-nameserver.c
        pdu_handler/client-nameserver/pdu_handler_client-nameserver.h
        pdu_handler/client-nameserver/test_pdu_handler_client-nameserver.c
        pdu_handler/client-nameserver/test_pdu_handler_client-nameserver.h
        pdu_handler/server-nameserver/pdu_handler_server-nameserver.c
        pdu_handler/server-nameserver/pdu_handler_server-nameserver.h
        pdu_handler/server-nameserver/test_pdu_handler_server-nameserver.c
        pdu_handler/server-nameserver/test_pdu_handler_server-nameserver.h
        pdu_handler/pdu_handler_client-server.c
        pdu_handler/pdu_handler_client-server.h
        pdu_handler/pdu_helper.c
        pdu_handler/pdu_helper.h
        pdu_handler/pdu_join_tests.c
        pdu_handler/pdu_join_tests.h
        pdu_handler/pdu_mess_tests.c
        pdu_handler/pdu_mess_tests.h
        pdu_handler/pdu_particpants_tests.c
        pdu_handler/pdu_particpants_tests.h
        pdu_handler/pdu_quit_tests.c
        pdu_handler/pdu_quit_tests.h
        pdu_handler/pdu_tests.c
        pdu_handler/pdu_tests.h
        error_handling.c
        error_handling.h
        main.c)
