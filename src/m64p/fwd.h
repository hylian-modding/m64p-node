#pragma once

#include <string>
#include <unordered_map>

namespace M64P {

namespace Cheat {

struct Entry;
struct Block;
using BlockMap = std::unordered_map<std::string, Block>;

}

class Core;
struct Error;
class Plugin;
struct Version;

}
