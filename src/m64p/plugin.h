#pragma once

#include "m64p/version.h"

#include <filesystem>

#include <m64p_api/m64p_types.h>

namespace M64P {

class Core;

class Plugin {
public:
    using DebugCallback = void(*)(void* context, int level, const char* message);

    Plugin();
    ~Plugin();

    void Load(const std::filesystem::path& path);
    void Unload();
    m64p_dynlib_handle GetHandle() const;

    m64p_plugin_type GetType();
    Version GetVersion();
    Version GetAPIVersion();
    std::string GetName();
    m64p_core_caps GetCaps();

    void Startup(const Core& core, void* context, DebugCallback debug_cb);
    void Shutdown();

private:
    class Loader;

    std::unique_ptr<Loader> m_ld;
};

}
