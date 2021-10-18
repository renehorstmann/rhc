cmake_minimum_required(VERSION 3.0)
project(rhc)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long -pedantic -Wno-unused-function -Wno-missing-braces")

include_directories(include)

add_executable(main examples/main.c)

add_executable(dynarray examples/dynarray.c)
add_executable(hashmap examples/hashmap.c)
add_executable(safety examples/safety.c)
add_executable(socket_server examples/socket_server.c)
add_executable(socket_client examples/socket_client.c)

if(WIN32)
    message("Windows")
    add_definitions(-DRHC_LOG_DO_NOT_USE_COLOR)
    target_compile_definitions(socket_server PRIVATE -DOPTION_SOCKET)
    target_compile_definitions(socket_client PRIVATE -DOPTION_SOCKET)
    target_link_libraries(socket_server ws2_32)
    target_link_libraries(socket_client ws2_32)
endif()
