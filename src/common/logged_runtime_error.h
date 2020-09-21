#pragma once

#include "common/logger.h"

#include <stdexcept>

struct LoggedRuntimeError : std::runtime_error {
    LoggedRuntimeError(const std::string& module, const std::string& message) :
        std::runtime_error{message}
    {
        Logger::Log(LogCategory::Error, module, message);
    }
};
