cmake_minimum_required(VERSION 3.14)
project(ProcMonitor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(ProcMonitor src/main.cpp)

target_include_directories(ProcMonitor PRIVATE include)
target_sources(ProcMonitor PRIVATE src/datagram_parser.cpp)
target_link_libraries(ProcMonitor PRIVATE nlohmann_json::nlohmann_json)

if(WIN32)
    target_sources(ProcMonitor PRIVATE src/process_lister_win.cpp src/udp_sender_win.cpp src/udp_reciever_win.cpp src/winsock_initializer_win.cpp)
    target_link_libraries(ProcMonitor PRIVATE psapi ws2_32)
    target_link_options(ProcMonitor PRIVATE "-static-libgcc" "-static-libstdc++" "-static")
else()
    target_sources(ProcMonitor PRIVATE src/process_lister_linux.cpp src/udp_sender_linux.cpp src/udp_reciever_linux.cpp src/winsock_initializer_linux.cpp)
endif()

enable_testing()

add_executable(DatagramParserTests
    tests/datagram_parser_test.cpp
    src/datagram_parser.cpp
)
target_include_directories(DatagramParserTests PRIVATE include)
target_link_libraries(DatagramParserTests PRIVATE nlohmann_json::nlohmann_json)
add_test(NAME DatagramParserTests COMMAND DatagramParserTests)
