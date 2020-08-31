#include "common/message_box.h"
#include "common/string_util.h"
#include "frontend/cheat_conf/edit.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "m64p/cheat.h"
#include "m64p/core.h"

#include <cassert>
#include <regex>
#include <sstream>

#include <fmt/format.h>

namespace CheatConf {

static int FilterName(ImGuiInputTextCallbackData* data)
{
    if (static_cast<std::string*>(data->UserData)->size() >= 30)
        return true;

    return !((data->EventChar >= 'a' && data->EventChar <= 'z')
        || (data->EventChar >= 'A' && data->EventChar <= 'Z')
        || (data->EventChar >= '0' && data->EventChar <= '9')
        || data->EventChar == ' ');
}

Edit::Edit(M64P::Core& core, M64P::Cheat::Block& block,
    M64P::Cheat::Entry* parent_entry, M64P::Cheat::Entry& entry) :
    m_core{&core},
    m_block{&block},
    m_parent_entry{parent_entry},
    m_entry{&entry}
{
    m_name = m_entry->name;
    m_codes = FormatCodes();
    m_options = FormatOptions();
}

bool Edit::Show(ImFont* mono_font)
{
    if (ImGui::GetWindowViewport()->PlatformHandle)
        m_window = static_cast<SDL_Window*>(ImGui::GetWindowViewport()->PlatformHandle);

    bool should_update{};

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {3.0f, 0.0f});
    if (ImGui::Button("Update", {100.0f, 0.0f}))
        should_update = true;
    ImGui::SameLine();
    if (ImGui::Button("Cancel", {100.0f, 0.0f}))
        m_open = false;
    ImGui::PopStyleVar();

    ImGui::SameLine();
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Name:");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(-1.0f);
    ImGui::InputText("##name", &m_name, ImGuiInputTextFlags_CallbackCharFilter, FilterName, &m_name);

    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
    ImGui::BeginChild("##frame", {}, true);
    ImGui::Columns(2, "cols", false);

    ImGui::SetColumnWidth(0, 147.0f);
    ImGui::TextUnformatted("Codes:\n<address> <value>");
    ImGui::PushFont(mono_font);
    ImGui::InputTextMultiline("##codes", &m_codes, {135.0f, -1.0f});
    ImGui::PopFont();

    ImGui::NextColumn();

    ImGui::TextUnformatted("Options:\n<value> <label>");
    ImGui::PushFont(mono_font);
    ImGui::SetNextItemWidth(-1.0f);
    ImGui::InputTextMultiline("##options", &m_options, {-1.0f, -1.0f});
    ImGui::PopFont();

    ImGui::EndChild();
    ImGui::PopStyleVar();

    if (should_update)
        Update();

    return m_open;
}

bool Edit::ValidateCodes(
    std::string text,
    std::vector<std::string>* addresses,
    std::vector<std::string>* values)
{
    if (text.empty()) return false;
    StringUtil::TrimNoCopy(text);
    const auto lines = StringUtil::Split(text, '\n');
    if (lines.empty()) return false;

    const auto is_valid_address = [](const std::string& value) {
        std::regex exp{"[0-9a-fA-F]{8}"};
        return value.size() == 8 && std::regex_match(value, exp);
    };

    const auto is_valid_value = [](const std::string& value) {
        std::regex exp{"[0-9a-fA-F]{4}"};
        return value.size() == 4 && (value == "????" || std::regex_match(value, exp));
    };

    for (auto& line : lines) {
        const auto words = StringUtil::Split(line, ' ');
        if (words.size() != 2) return false;
        const auto& address = words[0];
        const auto& value = words[1];

        if (!is_valid_address(address) || !is_valid_value(value))
            return false;

        if (addresses) (*addresses).push_back(address);
        if (values) (*values).push_back(value);
    }

    return true;
}

bool Edit::ValidateOptions(
    std::string text,
    std::vector<std::string>* values,
    std::vector<std::string>* labels)
{
    if (text.empty()) return false;
    StringUtil::TrimNoCopy(text);
    const auto lines = StringUtil::Split(text, '\n');
    if (lines.empty()) return false;

    const auto is_valid_value = [](const std::string& value) {
        std::regex exp{"[0-9a-fA-F]{4}"};
        return value.size() == 4 && std::regex_match(value, exp);
    };

    const auto is_valid_label = [](const std::string& value) {
        std::regex exp{"[a-zA-Z0-9 \\(\\)]*"};
        return std::regex_match(value, exp);
    };

    for (auto& line : lines) {
        auto space_index = line.find(' ');
        if (space_index == std::string::npos) return false;
        auto value = line.substr(0, space_index);
        auto label = line.substr(space_index + 1);

        if (!is_valid_value(value) || !is_valid_label(label))
            return false;

        if (values) (*values).push_back(value);
        if (labels) (*labels).push_back(label);
    }

    return true;
}

std::string Edit::FormatCodes()
{
    std::ostringstream oss;

    for (const auto& code : m_entry->codes) {
        if (code.value >= 0)
            oss << fmt::format("{:08X} {:04X}\n", code.address, code.value);
        else
            oss << fmt::format("{:08X} ????\n", code.address);
    }

    return oss.str();
}

std::string Edit::FormatOptions()
{
    std::ostringstream oss;

    for (const auto& option : m_entry->options)
        oss << fmt::format("{:04X} {}\n", option.second, option.first);

    return oss.str();
}

void Edit::ApplyCodes(
    const std::vector<std::string>& addresses,
    const std::vector<std::string>& values)
{
    assert(addresses.size() == values.size());

    m_entry->codes.clear();

    for (std::size_t i{}; i < addresses.size(); ++i) {
        const std::string& address = addresses[i];
        const std::string& value = values[i];
        m64p_cheat_code code;

        if (std::sscanf(address.c_str(), "%X", &code.address) == 1) {
            if (value == "????")
                m_entry->codes.push_back({code.address, -1});
            else if (std::sscanf(value.c_str(), "%X", &code.value) == 1)
                m_entry->codes.push_back(code);
        }
    }
}

void Edit::ApplyOptions(
    const std::vector<std::string>& values,
    const std::vector<std::string>& labels)
{
    assert(values.size() == labels.size());

    m_entry->options.clear();

    for (std::size_t i{}; i < values.size(); ++i) {
        const std::string& value = values[i];
        const std::string& label = labels[i];
        int code_value{};

        if (std::sscanf(value.c_str(), "%X", &code_value) == 1)
            m_entry->options[label] = code_value;
    }
}

void Edit::DisableEntry()
{
    if (m_entry->enabled) {
        std::string cheat_name;

        if (m_entry->option.has_value())
            cheat_name = fmt::format("C{:X}_{}", m_entry->id, m_entry->option.value());
        else
            cheat_name = fmt::format("C{:X}", m_entry->id);

        m_core->SetCheatEnabled(cheat_name, false);

        m_entry->enabled = false;
        m_entry->option = std::nullopt;
    }
}

void Edit::UpdateEntry()
{
    std::map<std::string, M64P::Cheat::Entry>* entries =
        m_parent_entry ? &m_parent_entry->entries : &m_block->entries;
    std::string name = StringUtil::Trim(m_name);

    auto node = entries->extract(m_entry->name);
    node.key() = name;
    node.mapped().name = name;
    entries->insert(std::move(node));
}

void Edit::Update()
{
    auto title = fmt::format("Cheat Edit Validation - {}", m_entry->name);
    std::map<std::string, M64P::Cheat::Entry>* entries =
        m_parent_entry ? &m_parent_entry->entries : &m_block->entries;
    std::string name = StringUtil::Trim(m_name);

    std::vector<std::string> codes_addresses, codes_values;
    std::vector<std::string> options_values, options_labels;
    bool valid_codes{}, valid_options{};

    if (name.empty()) {
        MessageBoxShow(m_window, MessageBoxIcon::Error,
            title, "Invalid name", "Name can't be empty");
        return;
    }

    if (name.size() > 50) {
        MessageBoxShow(m_window, MessageBoxIcon::Error,
            title, "Invalid name", "Name too long");
        return;
    }

    if (auto it = entries->find(name); it != entries->end() && it->second.id != m_entry->id) {
        MessageBoxShow(m_window, MessageBoxIcon::Error,
            title, "Invalid name", "Name already used");
        return;
    }

    if (!m_codes.empty()) {
        valid_codes = ValidateCodes(m_codes, &codes_addresses, &codes_values);

        if (!valid_codes)
            MessageBoxShow(m_window,
                valid_codes ? MessageBoxIcon::Information : MessageBoxIcon::Error,
                title, "Invalid codes", "Codes aren't formatted as expected");
    }

    if (!m_options.empty()) {
        valid_options = ValidateOptions(m_options, &options_values, &options_labels);

        if (!valid_options)
            MessageBoxShow(m_window,
                valid_options ? MessageBoxIcon::Information : MessageBoxIcon::Error,
                title, "Invalid options", "Options aren't formatted as expected");
    }

    if (m_codes.empty() && m_options.empty()) {
        DisableEntry();
        m_entry->codes.clear();
        m_entry->options.clear();
        UpdateEntry();
        m_open = false;
    }
    else if (!m_codes.empty() && valid_codes && !m_options.empty() && valid_options) {
        DisableEntry();
        ApplyCodes(codes_addresses, codes_values);
        ApplyOptions(options_values, options_labels);
        UpdateEntry();

        bool found = std::find_if(m_entry->codes.begin(), m_entry->codes.end(),
            [](const m64p_cheat_code& v) { return v.value == -1; }) != m_entry->codes.end();

        if (!found)
            MessageBoxShow(m_window, MessageBoxIcon::Warning,
                title, "No optional codes", "Options won't be applied as there are no optional codes");

        m_open = false;
    }
    else if (!m_codes.empty() && valid_codes && m_options.empty()) {
        bool found = std::find_if(codes_values.begin(), codes_values.end(),
            [](const std::string& v) { return v == "????"; }) != codes_values.end();

        if (found) {
            MessageBoxShow(m_window, MessageBoxIcon::Error,
                title, "Invalid codes", "No options for optional codes");
            return;
        }

        DisableEntry();
        ApplyCodes(codes_addresses, codes_values);
        m_entry->options.clear();
        UpdateEntry();

        m_open = false;
    }
    else if (m_codes.empty() && !m_options.empty() && valid_options) {
        MessageBoxShow(m_window, MessageBoxIcon::Error,
            title, "Invalid codes", "Options can't be added without codes");
    }
}

}
