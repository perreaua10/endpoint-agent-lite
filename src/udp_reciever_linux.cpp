#include "process_monitor/udp_reciever.h"
#include <iostream>
#include <cerrno>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h> // Core socket functions (socket, bind, listen)
#include <netinet/in.h> // Internet address structures (sockaddr_in)
#include <unistd.h>     // For close()

UdpReceiver::UdpReceiver(const std::string& bindIp, int bindPort) : buffer_(65536) {
    socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_ == kInvalidSocket) {
        std::cout << "Failed to create socket" << std::endl;
        return;
    }

    bindAddr_.sin_family = AF_INET;
    bindAddr_.sin_port = htons(bindPort);
    bindAddr_.sin_addr.s_addr = inet_addr(bindIp.c_str());
    // Cast sockaddr_in (IPv4) to the generic sockaddr type required by bind.
    if (bind(socket_, reinterpret_cast<sockaddr*>(&bindAddr_), sizeof(bindAddr_)) == -1) {
        std::cout << "Failed to bind socket: " << strerror(errno) << std::endl;
        return;
    }

    valid_ = true;
}

UdpReceiver::~UdpReceiver() {
    if (socket_ != kInvalidSocket) {
        close(socket_);
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

    const ssize_t received = recvfrom(
        socket_,
        buffer_.data(),
        buffer_.size(),
        0,
        nullptr,
        nullptr
    );

    if (received < 0) {
        return false;
    }

    data.assign(
        reinterpret_cast<const char*>(buffer_.data()),
        static_cast<std::size_t>(received)
    );

    return true;
}
