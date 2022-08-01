cmake_minimum_required(VERSION 3.0)
project(rhc)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(main examples/main.c)

add_executable(dynarray examples/dynarray.c)
add_executable(hashmap examples/hashmap.c)
add_executable(safety examples/safety.c)
add_executable(socket_server examples/socket_server.c)
add_executable(socket_client examples/socket_client.c)

target_compile_definitions(socket_server PRIVATE -DOPTION_SOCKET)
target_compile_definitions(socket_client PRIVATE -DOPTION_SOCKET)

# check platform
if (MINGW)
    message("Platform: Windows MinGW")
    add_definitions(-DPLATFORM_MINGW)
    set(PLATFORM_MINGW true)
elseif(WIN32)
    message("Platform: Windows MSVC")
    add_definitions(-DPLATFORM_MSVC)
    set(PLATFORM_MSVC true)
else()
    message("Platform: UNIX")
    add_definitions(-DPLATFORM_UNIX)
    set(PLATFORM_UNIX true)
endif()


if(PLATFORM_MINGW)
    message("Setup Windows MinGW")

    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4 /WX")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4267 /wd4244 /wd4305 /wd4996 /wd5105")

    target_link_libraries(socket_server ws2_32)
    target_link_libraries(socket_client ws2_32)
endif()

if(PLATFORM_MSVC)
    message("Setup Windows MSVC")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4 /WX")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244 /wd4305 /wd4996 /wd5105 /wd4100 /wd4200")

    target_link_libraries(socket_server ws2_32)
    target_link_libraries(socket_client ws2_32)
endif()

if(UNIX)
    message("Setup UNIX")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-long-long -Wno-unused-function -Wno-unused-variable -Wno-missing-braces -Wno-unused-parameter")

    # test sdl
    if(1)
        find_package(SDL2 REQUIRED)
        find_library(SDL2_NET_LIBRARIES SDL2_net REQUIRED)

        target_link_libraries(main
                ${SDL2_LIBRARIES}
                ${SDL2_NET_LIBRARIES}
                )
        target_link_libraries(socket_client
                ${SDL2_LIBRARIES}
                ${SDL2_NET_LIBRARIES}
                )
        target_link_libraries(socket_client
                ${SDL2_LIBRARIES}
                ${SDL2_NET_LIBRARIES}
                )
        target_compile_definitions(main PRIVATE -DOPTION_SDL)
        target_compile_definitions(socket_server PRIVATE -DOPTION_SDL)
        target_compile_definitions(socket_client PRIVATE -DOPTION_SDL)
    endif()
endif()