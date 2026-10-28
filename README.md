# Endpoint Agent Lite

A cross-platform C++20 endpoint-telemetry agent. It is a learning project that
is growing into a small, self-hosted endpoint-agent prototype.

## Current behavior

- Enumerates running processes on Windows and Linux.
- Collects each process ID, name, and memory usage.
- Sends one JSON process record per UDP datagram to `127.0.0.1:9999` every
  10 seconds.
- Listens for incoming UDP datagrams on `0.0.0.0:9000` in a background
  thread and prints received data.

The receiver currently proves the server-to-agent transport path. It does not
yet parse or act on commands.

## Layout

```text
include/process_monitor/   Shared public headers
src/                       Main program and platform implementations
udp_test_server_tools/     Small Python/JavaScript UDP test helpers
build/, windows/, linux/   Local CMake build output directories
```

## Build from WSL

Open WSL in the repository root (the folder containing `CMakeLists.txt` and
`README.md`) and run all commands below from there. Install CMake, the native
Linux build tools, and the MinGW cross-compiler once per WSL distribution:

```bash
sudo apt update
sudo apt install cmake build-essential \
  gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64
```

The first CMake configure downloads `nlohmann_json` from GitHub, so it needs
network access.

### Linux executable

```bash
cmake -S . -B build/wsl-linux
cmake --build build/wsl-linux
```

The executable is `build/wsl-linux/ProcMonitor`.

### Windows executable (cross-compiled with MinGW)

```bash
cmake -S . -B build/wsl-windows \
  -DCMAKE_SYSTEM_NAME=Windows \
  -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
  -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++
cmake --build build/wsl-windows
```

The executable is `build/wsl-windows/ProcMonitor.exe`. The C++ compiler name
ends in `-g++`.

## Build natively on Windows

Requires CMake and a C++20 compiler. From PowerShell at the repository root:

```powershell
cmake -S . -B build
cmake --build build
```

On Windows, run the built executable, then use the scripts in
`udp_test_server_tools/` to test the two UDP directions:

```text
Agent telemetry:     agent -> 127.0.0.1:9999
Agent command input: test tool -> 127.0.0.1:9000
```

## Next steps

- Define a versioned JSON message envelope for telemetry and commands.
- Add a small command processor, starting with read-only commands such as
  `ping` and `get_status`.
- Store received telemetry on the central server.
- Add event-driven Windows telemetry: process activity, selected registry
  changes, and Windows Event Log subscriptions.

## Security note

UDP command messages are not authenticated or encrypted yet. Do not add
destructive endpoint actions until the server and agent have an authenticated
command protocol.
