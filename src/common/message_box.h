#pragma once

#include <string>

struct SDL_Window;

enum class MessageBoxIcon {
	Error,
	Warning,
	Information,
    Question
};

void MessageBoxShow(SDL_Window* parent, MessageBoxIcon icon, const std::string& title,
    const std::string& inst, const std::string& content);
