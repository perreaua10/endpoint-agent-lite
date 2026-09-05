#include "process_monitor/process_lister.h"
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdint>

namespace fs = std::filesystem;


// Helper function to check if a string consists only of digits
bool isNumber(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}


bool ListAndSearchForProcesses(const std::wstring& processName) {
    int pid = 1;
    std::string path = "/proc/" + std::to_string(pid) + "/status";
    std::ifstream statusFile(path);

    if (!statusFile.is_open()) {
        std::cerr << "Could not open process data for PID: " << pid << " (Does it exist?)\n";
        return false;
    }

    std::string line;
    std::string procName;
    std::string vmSize;

    // Parse the file line by line
    while (std::getline(statusFile, line)) {
        if (line.rfind("Name:", 0) == 0) {       // Starts with "Name:"
            procName = line.substr(5);
        } else if (line.rfind("VmSize:", 0) == 0) { // Starts with "VmSize:"
            vmSize = line.substr(7);
        }
    }

    std::cout << "--- PID " << pid << " Metrics ---\n";
    std::cout << "Process Name: " << procName << "\n";
    std::cout << "Virtual Memory Size: " << vmSize << "\n";

    return true;
}

// Helper function to parse details from /proc/[pid]/status
bool getProcessDetails(uint32_t pid, std::string& outName, uint64_t& outMemKb) {
    std::ifstream statusFile("/proc/" + std::to_string(pid) + "/status");
    if (!statusFile.is_open()) {
        return false; // Process might have terminated before we could read it
    }

    std::string line;
    bool foundName = false;
    bool foundMem = false;
    
    // Default fallback values if fields aren't found
    outName = "Unknown";
    outMemKb = 0;

    while (std::getline(statusFile, line)) {
        // Parse the process Name
        if (line.rfind("Name:", 0) == 0) { 
            std::istringstream iss(line);
            std::string label;
            iss >> label >> outName;
            foundName = true;
        }
        // Parse Resident Set Size (Physical Memory usage in KB)
        else if (line.rfind("VmRSS:", 0) == 0) {
            std::istringstream iss(line);
            std::string label;
            iss >> label >> outMemKb; // Extracted automatically as uint64_t
            foundMem = true;
        }

        // Break early if we found both fields to optimize performance
        if (foundName && foundMem) {
            break;
        }
    }

    return true;
}

// Updated function returning the structured vector
std::vector<ProcessInfo> getAllProcesses() {
    std::vector<ProcessInfo> processList;
    const std::string procPath = "/proc";

    try {
        //check if process folder exists
        if (!fs::exists(procPath) || !fs::is_directory(procPath)) {
            std::cerr << "Error: " << procPath << " is inaccessible.\n";
            return processList;
        }

        for (const auto& entry : fs::directory_iterator(procPath)) {
            if (entry.is_directory()) {
                std::string filename = entry.path().filename().string();
                
                if (isNumber(filename)) {
                    uint32_t pid = static_cast<uint32_t>(std::stoul(filename));
                    
                    std::string name;
                    uint64_t memoryKb;
                    
                    // Only add the process if we successfully read its details
                    if (getProcessDetails(pid, name, memoryKb)) {
                        processList.push_back({pid, name, memoryKb});
                    }
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
    }

    return processList;
}

std::vector<ProcessInfo> getProcessList() {
    std::vector<ProcessInfo> processList = getAllProcesses();
    return processList;

}


