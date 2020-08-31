#pragma once

#include "common/types.h"

#include <filesystem>
#include <vector>

namespace FileUtil {

std::vector<u8> ReadAllBytes(const std::filesystem::path& path);

}
