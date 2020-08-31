#pragma once

#include "m64p/fwd.h"

#include <string>
#include <vector>

struct SDL_Window;
struct ImFont;

namespace CheatConf {

class Edit {
public:
	Edit(M64P::Core& core, M64P::Cheat::Block& block,
        M64P::Cheat::Entry* parent_entry, M64P::Cheat::Entry& entry);

	bool Show(ImFont* mono_font);

private:
	M64P::Core* m_core{};
	M64P::Cheat::Block* m_block{};
	M64P::Cheat::Entry* m_parent_entry{};
	M64P::Cheat::Entry* m_entry{};
	std::string m_name;
	std::string m_codes;
	std::string m_options;
	SDL_Window* m_window{};
    bool m_open{true};

	bool ValidateCodes(
		std::string text,
		std::vector<std::string>* addresses,
		std::vector<std::string>* values);
	bool ValidateOptions(
		std::string text,
		std::vector<std::string>* values,
		std::vector<std::string>* labels);
	std::string FormatCodes();
	std::string FormatOptions();
	void ApplyCodes(
		const std::vector<std::string>& addresses,
		const std::vector<std::string>& values);
	void ApplyOptions(
		const std::vector<std::string>& values,
        const std::vector<std::string>& labels);
	void DisableEntry();
	void UpdateEntry();
	void Update();
};

}
