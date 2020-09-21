#include "common/logger.h"

#include <array>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

static void InsertTime(std::ostream& os)
{
    time_t now;
    time(&now);

    using namespace std::chrono;
    int ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() % 1000ULL;

    char buf[20];
    strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%S", localtime(&now));

    os << buf << '.' << std::setw(3) << std::setfill('0') << ms;
}

constexpr std::array<const char*, 6> k_LogCategoryStr{"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
constexpr std::array<const char*, 6> k_LogCategoryCol{"\x1B[34m", "\x1B[36m", "\x1B[32m", "\x1B[33m", "\x1B[31m", "\x1B[35m"};
const char* k_InitialCol = "\033[0m";

void Logger::Log(LogCategory cat, const std::string& module, const std::string& str) {
    std::ostream* os = &std::cout;
    if (cat == LogCategory::Error || cat == LogCategory::Fatal)
        os = &std::cerr;

    *os << k_LogCategoryCol[static_cast<std::size_t>(cat)] << '[';
    InsertTime(*os);
    *os << "] [" << k_LogCategoryStr[static_cast<std::size_t>(cat)];
    *os << "] " << module << " - ";
    *os << k_InitialCol;
    *os << str << std::endl;
};
