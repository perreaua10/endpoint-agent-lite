#ifndef DATAGRAM_PARSER_H
#define DATAGRAM_PARSER_H

#include <optional>
#include <string>
#include <string_view>

struct Command {
    std::string id;
    std::string name;
};

struct DatagramParseResult {
    std::optional<Command> command;
    std::string error;
};

DatagramParseResult ParseCommandDatagram(std::string_view datagram);

#endif // DATAGRAM_PARSER_H
