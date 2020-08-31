#pragma once

#include "frontend/tool_base.h"
#include "m64p/fwd.h"
#include "sdl/fwd.h"

#include <memory>
#include <string>

struct SDL_Window;
struct ImFont;

namespace CheatConf {

class Edit;

class View : public Frontend::ToolBase {
public:
	View(M64P::Core& core, M64P::Cheat::Block& block, M64P::Cheat::Block& user_block);
    ~View();

	void Show(SDL::Window& main_win, ImFont* mono_font);
    void DisableAllEntries();

private:
	M64P::Core* m_core{};
	M64P::Cheat::Block* m_block{};
	M64P::Cheat::Block* m_user_block{};
	std::string m_user_entry_to_delete;
	M64P::Cheat::Entry* m_user_parent_entry_to_delete{};
	std::string m_new_name;
	std::size_t m_new_id{1'000'000};
	std::unique_ptr<Edit> m_edit{};

    void ShowTree(M64P::Cheat::Block* block, bool can_edit);
	void ShowTreeNode(M64P::Cheat::Entry& entry, M64P::Cheat::Entry* parent_entry, bool can_edit);
    void ShowTreeNodeGroup(M64P::Cheat::Entry& entry, M64P::Cheat::Entry* parent_entry, bool can_edit);
	void ShowTreeNodeOptions(M64P::Cheat::Entry& entry, M64P::Cheat::Entry* parent_entry, bool can_edit);
	void ShowTreeNodeSingle(M64P::Cheat::Entry& entry, M64P::Cheat::Entry* parent_entry, bool can_edit);
	void ShowItemContextMenu(M64P::Cheat::Entry* parent_entry, M64P::Cheat::Entry* entry, bool delete_only = false);
	void NewEntry();
	void DisableEntry(M64P::Cheat::Entry& entry);
	void DisableTreeNode(M64P::Cheat::Entry& entry);
};

}
