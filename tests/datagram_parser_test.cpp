#include <cassert>

#include "process_monitor/datagram_parser.h"

int main() {
    const auto valid = ParseCommandDatagram(R"({
        "version": 1,
        "kind": "command",
        "id": "test-001",
        "name": "ping",
        "args": {}
    })");

    assert(valid.command.has_value());
    assert(valid.command->id == "test-001");
    assert(valid.command->name == "ping");

    const auto wrongKind = ParseCommandDatagram(
        R"({"version":1,"kind":"telemetry","id":"test-002","name":"ping","args":{}})"
    );
    assert(!wrongKind.command.has_value());

    const auto missingArgs = ParseCommandDatagram(
        R"({"version":1,"kind":"command","id":"test-003","name":"ping"})"
    );
    assert(!missingArgs.command.has_value());
}
