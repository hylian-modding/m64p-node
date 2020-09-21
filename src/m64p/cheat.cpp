#include "common/string_util.h"
#include "m64p/cheat.h"

#include <fstream>

#include <fmt/format.h>

namespace M64P::Cheat {

static void ParseOptions(std::map<std::string, int>& options, const std::string& src)
{
    std::size_t off{};

    while (off < src.size()) {
        auto value = StringUtil::ReadUntil(src, off, ':');
        off += value.size() + 2;
        auto name = StringUtil::ReadUntil(src, off, '\"');
        off += name.size() + 2;

        int value_int{};
        if (std::sscanf(value.c_str(), "%X", &value_int) == 1)
            options[name] = value_int;
    }
}

BlockMap Load(const std::filesystem::path& path)
{
    std::ifstream ifs{path};

    if (!ifs)
        throw std::runtime_error{fmt::format("Failed to open file for reading '{}'", path.string())};

    std::string line;
    line.reserve(1000);
    BlockMap blocks;
    Block* curr_block{};
    Entry* curr_entry{};
    std::size_t id{};

    while (std::getline(ifs, line)) {
        if (line.empty() || StringUtil::StartsWith(line, "//"))
            continue;

        StringUtil::TrimNoCopy(line);

        if (line.empty())
            continue;

        if (StringUtil::StartsWith(line, "crc")) {
            auto crc = line.substr(4);

            curr_block = &blocks[crc];
            curr_block->crc = crc;
        }
        else if (curr_block) {
            if (StringUtil::StartsWith(line, "gn")) {
                auto gn = line.substr(3);
                curr_block->good_name = gn;
            }
            else if (StringUtil::StartsWith(line, "cn")) {
                auto cn = line.substr(3);
                auto res = StringUtil::Split(cn, '\\');

                curr_entry = &curr_block->entries[res[0]];
                curr_entry->name = res[0];
                curr_entry->id = id++;

                for (std::size_t i = 1; i < res.size(); ++i) {
                    curr_entry = &curr_entry->entries[res[i]];
                    curr_entry->name = res[i];
                    curr_entry->id = id++;
                }
            }
            else if (curr_entry) {
                if (StringUtil::StartsWith(line, "cd")) {
                    auto cd = line.substr(3);
                    curr_entry->desc = cd;
                }
                else {
                    auto address = line.substr(0, 8);
                    auto value = line.substr(9, 4);
                    m64p_cheat_code code{};

                    if (std::sscanf(address.c_str(), "%X", &code.address) == 1) {
                        if (std::sscanf(value.c_str(), "%X", &code.value) == 1) {
                            curr_entry->codes.push_back(code);
                        }
                        else if (value == "????") {
                            curr_entry->codes.push_back({code.address, -1});

                            if (line.size() > 13) {
                                auto options = line.substr(14);

                                if (!options.empty())
                                    ParseOptions(curr_entry->options, options);
                            }
                        }
                    }
                }
            }
        }
    }

    return blocks;
}

static void SaveNode(std::ofstream& ofs, const std::map<std::string, Entry>& entries, std::string name)
{
    for (auto& [_, cheat] : entries) {
        if (cheat.entries.empty()) {
            if (name.empty())
                ofs << fmt::format(" cn {}\n", cheat.name);
            else
                ofs << fmt::format(" cn {}\\{}\n", name, cheat.name);

            if (!cheat.desc.empty())
                ofs << fmt::format("  cd {}\n", cheat.desc);

            std::optional<std::size_t> option_index{};
            for (std::size_t i{}; i < cheat.codes.size(); ++i) {
                if (cheat.codes[i].value < 0)
                    option_index = i;
            }

            for (std::size_t i{}; i < cheat.codes.size(); ++i) {
                const auto& code = cheat.codes[i];

                if (code.value >= 0) {
                    ofs << fmt::format("  {:08X} {:04X}\n", code.address, code.value);
                }
                else {
                    if (i != option_index) {
                        ofs << fmt::format("  {:08X} ????\n", code.address);
                    }
                    else {
                        ofs << fmt::format("  {:08X} ???? ", code.address);

                        for (std::size_t i{}; i < cheat.options.size(); ++i) {
                            auto it = cheat.options.begin();
                            std::advance(it, i);

                            ofs << fmt::format("{:04X}:\"{}\"", it->second, it->first);

                            if (i < cheat.options.size() - 1)
                                ofs << ',';
                            else
                                ofs << '\n';
                        }
                    }
                }
            }
        }
        else {
            if (name.empty())
                SaveNode(ofs, cheat.entries, cheat.name);
            else
                SaveNode(ofs, cheat.entries, fmt::format("{}\\{}", name, cheat.name));
        }
    }
}

void Save(const std::filesystem::path& path, const BlockMap& blocks)
{
    std::ofstream ofs{path};

    if (!ofs)
        throw std::runtime_error{fmt::format("Failed to open file for writing {}", path.string())};

    for (auto& [_, game] : blocks) {
        ofs << fmt::format("crc {}\n", game.crc);
        ofs << fmt::format("gn {}\n", game.good_name);

        std::string name;
        SaveNode(ofs, game.entries, name);
    }
}

}
