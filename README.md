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
windows/, linux/           Local CMake build output directories
```

## Build

Requires CMake and a C++20 compiler.

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
