#include "common/file_util.h"

#include <fstream>
#include <stdexcept>

#include <fmt/format.h>

namespace FileUtil {

std::vector<u8> ReadAllBytes(const std::filesystem::path& path)
{
    std::ifstream ifs{path, std::ios::binary | std::ios::ate};

    if (!ifs)
        throw std::runtime_error{fmt::format("Failed to open file '{}'", path.string())};

    auto len = ifs.tellg();
    ifs.seekg(0);

    std::vector<u8> v(static_cast<std::size_t>(len));
    ifs.read(reinterpret_cast<char*>(v.data()), len);

    return v;
}

}
