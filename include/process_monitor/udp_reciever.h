#ifndef UDP_RECEIVER_H
#define UDP_RECEIVER_H
#include <string>
#include <cstddef>
#include <vector>
#ifdef _WIN32
    #include <winsock2.h>
    using SocketHandle = SOCKET;
    constexpr SocketHandle kInvalidSocket = INVALID_SOCKET;
#else
    #include <netinet/in.h>
    using SocketHandle = int;
    constexpr SocketHandle kInvalidSocket = -1;
#endif



class UdpReceiver {
  public:
      UdpReceiver(const std::string& bindIp, int bindPort);
      ~UdpReceiver();

      bool IsValid() const;
      bool Receive(std::string& data);

  private:
    SocketHandle socket_ = kInvalidSocket;
    struct sockaddr_in bindAddr_{};
    bool valid_ = false;
    std::vector<std::byte> buffer_;
  };
#endif // UDP_RECEIVER_H
