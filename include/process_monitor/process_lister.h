#ifndef PROCESS_LISTER_H
#define PROCESS_LISTER_H
#include <vector>
#include <string>
#include <sstream>
#include <cstdint>

struct ProcessInfo {
    uint32_t pid;
    std::string name;
    uint64_t memoryUsageKb;

    std::string ToJson() const {
        std::ostringstream json;
        json << "{\"pid\":" << pid << ",\"name\":\"" << name << "\",\"memoryUsageKb\":" << memoryUsageKb << "}";
        return json.str();
    }
};
// Lists all running processes and searches for the specified process name
bool ListAndSearchForProcesses(const std::wstring& processName);
// Returns a list of all running processes
std::vector<ProcessInfo> getProcessList();

#endif // PROCESS_LISTER_H
