#include "common/message_box.h"
#include "common/string_util.h"
#include "frontend/cheat_conf/edit.h"
#include "frontend/cheat_conf/view.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_internal.h"
#include "m64p/cheat.h"
#include "m64p/core.h"

#include <fmt/format.h>

namespace CheatConf {

const ImVec4 k_TreeNodeColor = {0.809f, 0.809f, 0.809f, 1.0f};
const ImVec4 k_CheatEnabledColor = {0.5f, 0.8f, 0.5f, 1.0f};
const ImVec4 k_CheatWarnColor = {0.9f, 0.9f, 0.5f, 1.0f};

static int FilterName(ImGuiInputTextCallbackData* data)
{
    if (static_cast<std::string*>(data->UserData)->size() >= 200)
        return true;

    return !((data->EventChar >= 'a' && data->EventChar <= 'z')
        || (data->EventChar >= 'A' && data->EventChar <= 'Z')
        || (data->EventChar >= '0' && data->EventChar <= '9')
        || data->EventChar == ' '
        || data->EventChar == '\\');
}

View::View(M64P::Core& core, M64P::Cheat::Block& block, M64P::Cheat::Block& user_block) :
    m_core{&core},
    m_block{&block},
    m_user_block{&user_block}
{}

View::~View() = default;

void View::Show(SDL::Window& main_win, ImFont* mono_font)
{
    if (!m_open)
        return;

    ImGui::SetNextWindowSize({640.0, 480.0f}, ImGuiCond_Appearing);
    ImGui::Begin("Cheats", &m_open, GetWindowFlags(main_win) | ImGuiWindowFlags_NoDocking);

    CommonBehavior();

    if (!m_edit) {
        if (ImGui::Button("New", {100.0f, 0.0f}))
            NewEntry();

        ImGui::SameLine();
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted("Name:");

        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1.0f);
        ImGui::InputText("##name", &m_new_name, ImGuiInputTextFlags_CallbackCharFilter, FilterName, &m_new_name);

        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        ImGui::BeginChild("##frame", {}, true);

        if (!m_user_block->entries.empty()) {
            if (ImGui::CollapsingHeader("User", ImGuiTreeNodeFlags_DefaultOpen))
                ShowTree(m_user_block, true);
        }

        if (!m_block->entries.empty()) {
            if (ImGui::CollapsingHeader("Built-in", ImGuiTreeNodeFlags_DefaultOpen))
                ShowTree(m_block, false);
        }

        ImGui::EndChild();
        ImGui::PopStyleVar();
    }
    else {
        if (!m_open || !m_edit->Show(mono_font)) {
            m_open = true;
            m_edit.reset();
        }
    }

    ImGui::End();

    if (!m_user_entry_to_delete.empty()) {
        if (m_user_parent_entry_to_delete) {
            DisableTreeNode(m_user_parent_entry_to_delete->entries[m_user_entry_to_delete]);
            m_user_parent_entry_to_delete->entries.erase(m_user_entry_to_delete);
        }
        else {
            DisableTreeNode(m_user_block->entries[m_user_entry_to_delete]);
            m_user_block->entries.erase(m_user_entry_to_delete);
        }

        m_user_entry_to_delete.clear();
        m_user_parent_entry_to_delete = nullptr;
    }
}

void View::ShowTree(M64P::Cheat::Block* block, bool can_edit)
{
    M64P::Cheat::Entry* parent_entry{};

    for (auto& [_, v] : block->entries)
        ShowTreeNode(v, parent_entry, can_edit);
}

void View::ShowTreeNode(M64P::Cheat::Entry& entry, M64P::Cheat::Entry* parent_entry, bool can_edit)
{
    if (!entry.entries.empty())
        ShowTreeNodeGroup(entry, parent_entry, can_edit);
    else if (!entry.options.empty())
        ShowTreeNodeOptions(entry, parent_entry, can_edit);
    else
        ShowTreeNodeSingle(entry, parent_entry, can_edit);
}

void View::ShowTreeNodeGroup(M64P::Cheat::Entry& entry, M64P::Cheat::Entry* parent_entry, bool can_edit)
{
    ImGui::PushID(&entry);

    ImGui::PushStyleColor(ImGuiCol_Text, k_TreeNodeColor);
    bool shown = ImGui::TreeNodeEx(entry.name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth, "%s", entry.name.c_str());
    ImGui::PopStyleColor();

    if (can_edit)
        ShowItemContextMenu(parent_entry, &entry, true);

    if (shown) {
        for (auto& [_, v] : entry.entries)
            ShowTreeNode(v, &entry, can_edit);

        ImGui::TreePop();
    }

    ImGui::PopID();
}

void View::ShowTreeNodeOptions(M64P::Cheat::Entry& entry, M64P::Cheat::Entry* parent_entry, bool can_edit)
{
    ImGui::PushID(&entry);

    bool was_enabled = entry.option.has_value();
    auto menu_name = !entry.option.has_value() ?
        fmt::format("{} (? => Not set)", entry.name) :
        fmt::format("{} ({:04X} => {})", entry.name, entry.options.at(entry.option.value()), entry.option.value());
    auto popup_id = fmt::format("{}_popup", menu_name);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetCurrentContext()->Style.FramePadding.x);

    if (was_enabled)
        ImGui::PushStyleColor(ImGuiCol_Text, k_CheatEnabledColor);

    if (ImGui::MenuItem(menu_name.c_str()))
        ImGui::OpenPopup(popup_id.c_str());

    if (was_enabled)
        ImGui::PopStyleColor();

    if (can_edit)
        ShowItemContextMenu(parent_entry, &entry);

    if (ImGui::BeginPopup(popup_id.c_str())) {
        for (const auto& [name, _] : entry.options) {
            bool selected = entry.option == name;
            bool was_selected = selected;

            if (was_selected)
                ImGui::PushStyleColor(ImGuiCol_Text, k_CheatEnabledColor);

            if (ImGui::MenuItem(name.c_str(), nullptr, &selected)) {
                if (entry.enabled && entry.option.has_value()) {
                    auto curr_cheat_name = fmt::format("C{:X}_{}", entry.id, entry.option.value());
                    m_core->SetCheatEnabled(curr_cheat_name, false);
                }

                entry.option = name;
                entry.enabled = true;

                auto cheat_name = fmt::format("C{:X}_{}", entry.id, entry.option.value());
                auto codes = entry.codes;
                int value = entry.options.at(entry.option.value());

                for (auto& code : codes) {
                    if (code.value < 0)
                        code.value = value;
                }

                if (selected) {
                    m_core->AddCheat(cheat_name, codes.data(), codes.size());
                    m_core->SetCheatEnabled(cheat_name, selected);
                }
                else {
                    entry.option = std::nullopt;
                    entry.enabled = false;
                    m_core->SetCheatEnabled(cheat_name, false);
                }
            }

            if (was_selected)
                ImGui::PopStyleColor();

            if (was_selected && !selected) {
                entry.option = std::nullopt;
                entry.enabled = false;
            }
        }

        ImGui::EndPopup();
    }

    ImGui::PopID();
}

void View::ShowTreeNodeSingle(M64P::Cheat::Entry& entry, M64P::Cheat::Entry* parent_entry, bool can_edit)
{
    ImGui::PushID(&entry);

    auto cheat_name = fmt::format("C{:X}", entry.id);
    bool was_enabled = entry.enabled;

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetCurrentContext()->Style.FramePadding.x);

    if (entry.codes.empty())
        ImGui::PushStyleColor(ImGuiCol_Text, k_CheatWarnColor);

    if (was_enabled)
        ImGui::PushStyleColor(ImGuiCol_Text, k_CheatEnabledColor);

    if (ImGui::MenuItem(entry.name.c_str(), nullptr, &entry.enabled)) {
        if (!entry.codes.empty()) {
            if (entry.enabled)
                m_core->AddCheat(cheat_name, entry.codes.data(), entry.codes.size());

            m_core->SetCheatEnabled(cheat_name, entry.enabled);
        }
        else
            entry.enabled = false;
    }

    if (was_enabled)
        ImGui::PopStyleColor();

    if (entry.codes.empty())
        ImGui::PopStyleColor();

    if (can_edit)
        ShowItemContextMenu(parent_entry, &entry);

    ImGui::PopID();
}

void View::ShowItemContextMenu(M64P::Cheat::Entry* parent_entry, M64P::Cheat::Entry* entry, bool delete_only)
{
    auto popup_id = fmt::format("{}_p{}", entry->name, entry->id);

    ImGui::OpenPopupContextItem(popup_id.c_str());

    if (ImGui::BeginPopupContextItem(popup_id.c_str())) {
        if (!delete_only) {
            if (ImGui::Selectable("Edit", false, 0, { 150.0f, 0.0f }))
                m_edit = std::make_unique<Edit>(*m_core, *m_user_block, parent_entry, *entry);
        }

        if (ImGui::Selectable("Delete", false, 0, { 150.0f, 0.0f })) {
            m_user_entry_to_delete = entry->name;
            m_user_parent_entry_to_delete = parent_entry;
        }

        ImGui::EndPopup();
    }
}

void View::NewEntry() {
    auto title = "Cheats Validation";
    auto new_name = StringUtil::Trim(m_new_name);

    if (new_name.empty()) {
        MessageBoxShow(m_window, MessageBoxIcon::Error,
            title, "Invalid name", "Name can't be empty");
        return;
    }

    auto res = StringUtil::Split(new_name, '\\');

    if (std::any_of(res.begin(), res.end(), [](const std::string& v)
        { return v.empty() || std::all_of(v.begin(), v.end(), [](char c) { return c == ' '; }); })) {

        MessageBoxShow(m_window, MessageBoxIcon::Error,
            title, "Invalid name", "Name can't have empty nodes");
        return;
    }

    M64P::Cheat::Entry* entry, *parent_entry{};
    std::map<std::string, M64P::Cheat::Entry>* entries = &m_user_block->entries;
    std::vector<M64P::Cheat::Entry*> entries_to_disable;

    for (std::size_t i{}; i < res.size(); ++i) {
        bool is_leaf = i == res.size() - 1;
        StringUtil::TrimNoCopy(res[i]);

        if (res[i].size() >= 30) {
            MessageBoxShow(m_window, MessageBoxIcon::Error,
                title, "Invalid name", "Name too long");
            return;
        }

        if (auto it = entries->find(res[i]); it != entries->end()) {
            if (is_leaf) {
                MessageBoxShow(m_window, MessageBoxIcon::Error,
                    title, "Invalid name", "Name already used");
                return;
            }

            parent_entry = entry;
            entry = &it->second;
            entries = &entry->entries;

            if (entry->enabled)
                entries_to_disable.push_back(entry);
        }
        else {
            entry = &(*entries)[res[i]];
            entry->id = ++m_new_id;
            entry->name = res[i];
            entries = &entry->entries;
        }
    }

    for (auto i : entries_to_disable)
        DisableEntry(*i);

    m_new_name.clear();

    if (entry)
        m_edit = std::make_unique<Edit>(*m_core, *m_user_block, parent_entry, *entry);
}

void View::DisableTreeNode(M64P::Cheat::Entry& entry)
{
    if (!entry.entries.empty()) {
        for (auto& [_, v] : entry.entries)
            DisableTreeNode(v);
    }
    else
        DisableEntry(entry);
}

void View::DisableEntry(M64P::Cheat::Entry& entry)
{
    if (entry.enabled) {
        std::string cheat_name;

        if (entry.option.has_value())
            cheat_name = fmt::format("C{:X}_{}", entry.id, entry.option.value());
        else
            cheat_name = fmt::format("C{:X}", entry.id);

        m_core->SetCheatEnabled(cheat_name, false);

        entry.enabled = false;
        entry.option = std::nullopt;
    }
}

void View::DisableAllEntries()
{
    for (auto& [_, v] : m_user_block->entries)
        DisableTreeNode(v);

    for (auto& [_, v] : m_block->entries)
        DisableTreeNode(v);
}

}
