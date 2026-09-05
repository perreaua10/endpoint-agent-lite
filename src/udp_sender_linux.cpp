#include "process_monitor/udp_sender.h"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h> // Core socket functions (socket, bind, listen)
#include <netinet/in.h> // Internet address structures (sockaddr_in)
#include <unistd.h>     // For close()


UdpSender::UdpSender(const std::string& targetIp, int targetPort) {
    // constructor body
    socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_ == -1) {
        std::cout << "Failed to create socket" << std::endl;
        valid_ = false;
        return;
    }
    valid_ = true;
    targetAddr_.sin_family = AF_INET;
    targetAddr_.sin_port = htons(targetPort);
    targetAddr_.sin_addr.s_addr = inet_addr(targetIp.c_str());
}

void UdpSender::Send(const std::string& data) {
    int result = sendto(socket_, data.c_str(), data.size(), 0, (struct sockaddr*)&targetAddr_, sizeof(targetAddr_));
    if (result == -1) {
        // Handle error
        std::cout << "Send failed with error: " << strerror(errno) << std::endl;
    }

}

UdpSender::~UdpSender() {
    if (socket_ != -1) {
        close(socket_);
    }
}