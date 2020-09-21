#pragma once

#include <string>

enum class LogCategory {
	Trace,
	Debug,
	Info,
	Warn,
	Error,
	Fatal
};

struct Logger {
	static void Log(LogCategory cat, const std::string& module, const std::string& str);
};
