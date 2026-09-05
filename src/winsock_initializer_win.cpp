// src/winsock_initializer_win.cpp
#include "process_monitor/winsock_initializer.h"
#include <winsock2.h>
#include <iostream>

WinsockInitializer::WinsockInitializer() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cout << "WSAStartup failed: " << result << std::endl;
    }
}

WinsockInitializer::~WinsockInitializer() {
    WSACleanup();
}