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
constexpr std::array<const char*, 6> k_LogCategoryCol{"\x1B[34m", "\x1B[36m", "\x1B[32m", "\x1B[33m", "\x1b[1m\x1B[31m", "\x1B[35m"};
const char* k_InitialCol = "\033[0m";

#ifndef _WIN32

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

#else

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

constexpr std::array<WORD, 6> k_LogCategoryCol32{
    FOREGROUND_BLUE,
    FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_GREEN,
    FOREGROUND_RED | FOREGROUND_GREEN,
    FOREGROUND_RED | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_BLUE
};

static WORD GetConAttrs(HANDLE hConsoleOutput) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &info);
    return info.wAttributes;
}

void Logger::Log(LogCategory cat, const std::string& module, const std::string& str) {
    std::ostream* os = &std::cout;
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    if (cat == LogCategory::Error || cat == LogCategory::Fatal) {
        os = &std::cerr;
        hConsoleOutput = GetStdHandle(STD_ERROR_HANDLE);
    }

    const WORD con_attr = GetConAttrs(hConsoleOutput);
    const WORD col = con_attr & ~0xf;

    SetConsoleTextAttribute(hConsoleOutput, col | k_LogCategoryCol32[static_cast<std::size_t>(cat)]);
    *os << k_LogCategoryCol[static_cast<std::size_t>(cat)];
    *os << '[';
    InsertTime(*os);
    *os << "] [" << k_LogCategoryStr[static_cast<std::size_t>(cat)];
    *os << "] " << module << " - ";
    SetConsoleTextAttribute(hConsoleOutput, con_attr);
    *os << k_InitialCol;
    *os << str << std::endl;
};

#endif
