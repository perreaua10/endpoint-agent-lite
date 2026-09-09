#include "process_monitor/datagram_parser.h"

#include <cstdint>

#include <nlohmann/json.hpp>

namespace {

constexpr std::size_t kMaxCommandDatagramSize = 4096;

DatagramParseResult Invalid(std::string error) {
    return {std::nullopt, std::move(error)};
}

bool IsValidStringField(
    const nlohmann::json& message,
    const char* fieldName,
    std::size_t maxLength
) {
    const auto field = message.find(fieldName);
    return field != message.end()
        && field->is_string()
        && !field->get_ref<const std::string&>().empty()
        && field->get_ref<const std::string&>().size() <= maxLength;
}

} // namespace

DatagramParseResult ParseCommandDatagram(std::string_view datagram) {
    if (datagram.empty() || datagram.size() > kMaxCommandDatagramSize) {
        return Invalid("Command datagram must be between 1 and 4096 bytes.");
    }

    const auto message = nlohmann::json::parse(
        datagram.begin(),
        datagram.end(),
        nullptr,
        false
    );

    if (message.is_discarded() || !message.is_object()) {
        return Invalid("Command datagram must contain a JSON object.");
    }

    const auto version = message.find("version");
    if (version == message.end()
        || !version->is_number_integer()
        || version->get<std::int64_t>() != 1) {
        return Invalid("Command version must be the integer 1.");
    }

    const auto kind = message.find("kind");
    if (kind == message.end()
        || !kind->is_string()
        || kind->get_ref<const std::string&>() != "command") {
        return Invalid("Command kind must be 'command'.");
    }

    if (!IsValidStringField(message, "id", 128)) {
        return Invalid("Command id must be a non-empty string up to 128 bytes.");
    }

    if (!IsValidStringField(message, "name", 64)) {
        return Invalid("Command name must be a non-empty string up to 64 bytes.");
    }

    const auto args = message.find("args");
    if (args == message.end() || !args->is_object()) {
        return Invalid("Command args must be a JSON object.");
    }

    return {{
        message.at("id").get<std::string>(),
        message.at("name").get<std::string>()
    }, ""};
}
