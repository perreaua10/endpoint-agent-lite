#include <iostream>
#include <thread>
#include <stop_token>
#include <syncstream>
#include <chrono>
#include "process_monitor/process_lister.h"
#include "process_monitor/winsock_initializer.h"
#include "process_monitor/udp_sender.h"
#include "process_monitor/udp_reciever.h"

int main() {
    // Note: Use the L prefix for wide strings (wstring)
    // std::wstring target = L"chrome.exe"; 

    // if (ListAndSearchForProcesses(target)) {
    //     std::wcout << L"Found active instance of: " << target << std::endl;
    // } else {
    //     std::wcout << L"Could not find: " << target << std::endl;
    // }

    //Windows Specific code, eventually should be refactored for platform-agnostic
    WinsockInitializer winsockInit; // Ensure Winsock is initialized before using UDP sockets.
    UdpSender sender("127.0.0.1", 9999);
    UdpReceiver receiver("0.0.0.0", 9000);

    if (!sender.IsValid() || !receiver.IsValid()) {
        std::cerr << "Failed to initialize UDP sender or receiver.\n";
        return 1;
    }

    // Start a worker thread.
    // The lambda captures `receiver` by reference so it uses the existing UdpReceiver object
    // instead of copying it. std::jthread automatically passes a std::stop_token into the
    // lambda, which lets the worker cooperatively exit when stop is requested.
    std::jthread receiverThread([&receiver](std::stop_token stopToken) {
        std::string data;

        while (!stopToken.stop_requested()) {
            if (receiver.Receive(data)) {
                std::osyncstream(std::cout)
                    << "[receiver] received: "
                    << data
                    << '\n';
            }
        }
    });

    
    while (true) {
        const auto processes = getProcessList();
        for (const auto& p : processes) {
            sender.Send(p.ToJson());
        }

        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}
