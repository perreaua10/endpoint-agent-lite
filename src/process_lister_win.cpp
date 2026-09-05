#include "process_monitor/process_lister.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <psapi.h>
#include <tlhelp32.h>
#include <cstdint>

std::string WideStringToUtf8(const std::wstring& wide) {
    if (wide.empty()) return std::string();

    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), (int)wide.size(), nullptr, 0, nullptr, nullptr);
    std::string result(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), (int)wide.size(), &result[0], sizeNeeded, nullptr, nullptr);
    return result;
}

std::wstring getProcessMemoryUsage(DWORD processID) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess == NULL) {
        DWORD err = GetLastError();
        if (err == ERROR_ACCESS_DENIED) {
            std::wcout << L"Access denied to PID " << processID << std::endl;
            return L"Access denied";
        }
        std::wcout << L"Failed to open PID " << processID << L", error: " << err << std::endl;
        return L"Unable to open process";
    }

    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        SIZE_T memoryUsage = pmc.WorkingSetSize; // Memory usage in bytes
        CloseHandle(hProcess);
        return std::to_wstring(memoryUsage / 1024) + L" KB"; // Convert to KB
    } else {
        CloseHandle(hProcess);
        return L"Unable to retrieve memory info";
    }
}

uint64_t getProcessMemoryUsageKb(DWORD processID) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess == NULL) {
        return 0; // or some other agreed sentinel for "couldn't determine"
    }

    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        CloseHandle(hProcess);
        return pmc.WorkingSetSize / 1024;
    }

    CloseHandle(hProcess);
    return 0;
}

bool ListAndSearchForProcesses(const std::wstring& processName) {
    bool exists = false;
    
    // Take a snapshot of all running processes
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);

    // Step through the process list
    if (Process32FirstW(hSnapshot, &pe)) {
        do {
            std::wcout << L"Checking process: " << pe.szExeFile <<"\n" << L"Process ID: " << pe.th32ProcessID << std::endl;
            std::wstring memoryUsage = getProcessMemoryUsage(pe.th32ProcessID);
            std::wcout << L"Memory usage: " << memoryUsage << std::endl;
            if (processName == pe.szExeFile) {
                exists = true;
                break;
            }
        } while (Process32NextW(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);
    return exists;
}

std::vector<ProcessInfo> getProcessList() {
    std::vector<ProcessInfo> processList;
    
    // Take a snapshot of all running processes
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return processList; // Return empty list on failure
    }

    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);

    // Step through the process list
    if (Process32FirstW(hSnapshot, &pe)) {
        do {
            ProcessInfo info;
            info.pid = pe.th32ProcessID;
            info.name = WideStringToUtf8(std::wstring(pe.szExeFile));
            info.memoryUsageKb = getProcessMemoryUsageKb(pe.th32ProcessID);
            processList.push_back(info);
        } while (Process32NextW(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);
    return processList;
}
