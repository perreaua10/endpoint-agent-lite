cmake_minimum_required(VERSION 3.10)
project(ProcMonitor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ProcMonitor src/main.cpp)

target_include_directories(ProcMonitor PRIVATE include)

if(WIN32)
    target_sources(ProcMonitor PRIVATE src/process_lister_win.cpp src/udp_sender_win.cpp src/udp_reciever_win.cpp src/winsock_initializer_win.cpp)
    target_link_libraries(ProcMonitor PRIVATE psapi ws2_32)
    target_link_options(ProcMonitor PRIVATE "-static-libgcc" "-static-libstdc++" "-static")
else()
    target_sources(ProcMonitor PRIVATE src/process_lister_linux.cpp src/udp_sender_linux.cpp src/udp_reciever_linux.cpp src/winsock_initializer_linux.cpp)
endif()
