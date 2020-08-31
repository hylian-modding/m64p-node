#pragma once

#include <m64p_api/m64p_types.h>

#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace M64P::Cheat {

struct Entry {
    std::map<std::string, Entry> entries;
    std::string name;
    std::string desc;
    std::vector<m64p_cheat_code> codes;
    std::map<std::string, int> options;
    std::optional<std::string> option;
    bool enabled;
    std::size_t id;
};

struct Block {
    std::string crc;
    std::string good_name;
    std::map<std::string, Entry> entries;
};

using BlockMap = std::unordered_map<std::string, Block>;

BlockMap Load(const std::filesystem::path& path);
void Save(const std::filesystem::path& path, const BlockMap& blocks);

}
