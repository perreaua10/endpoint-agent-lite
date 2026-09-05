#include "process_monitor/udp_reciever.h"
#include <iostream>
#include <winsock2.h>

UdpReceiver::UdpReceiver(const std::string& bindIp, int bindPort) :buffer_(65536) { //this init notation always makes buffer initialized with this given amount of bytes
    // constructor body
    socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_ == kInvalidSocket) {
        std::cout << "Failed to create socket, error: " << WSAGetLastError() << std::endl;
        valid_ = false;
        return;
    }

    bindAddr_.sin_family = AF_INET;
    bindAddr_.sin_port = htons(bindPort);
    bindAddr_.sin_addr.s_addr = inet_addr(bindIp.c_str());
    if (bind(socket_, reinterpret_cast<sockaddr*>(&bindAddr_), sizeof(bindAddr_)) == SOCKET_ERROR) {
        std::cout << "Failed to bind socket, error: " << WSAGetLastError() << std::endl;
        valid_ = false;
        return;
    }
    valid_ = true;
    return;
}

UdpReceiver::~UdpReceiver() {
    if (socket_ != kInvalidSocket) {
        closesocket(socket_);
    }
}

bool UdpReceiver::IsValid() const {
    return valid_;
}

bool UdpReceiver::Receive(std::string& data)
{
    if (!valid_ || socket_ == kInvalidSocket) {
        return false;
    }

    const int received = recvfrom(
        socket_,
        reinterpret_cast<char*>(buffer_.data()),
        static_cast<int>(buffer_.size()),
        0,
        nullptr,
        nullptr
    );

    if (received == SOCKET_ERROR) {
        return false;
    }

    data.assign(
        reinterpret_cast<const char*>(buffer_.data()),
        static_cast<std::size_t>(received)
    );

    return true;
}
