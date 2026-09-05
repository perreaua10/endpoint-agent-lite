#ifndef UDP_SENDER_H
#define UDP_SENDER_H
#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <netinet/in.h>
#endif
#include <string>

class UdpSender {
public:
    UdpSender(const std::string& targetIp, int targetPort);
    ~UdpSender();

    void Send(const std::string& data);
    bool IsValid() const { return valid_; }
private:
    int socket_;
    struct sockaddr_in targetAddr_;
    bool valid_ = false;
};

#endif