#include "m64p/core.h"
#include "m64p/error.h"
#include "m64p/plugin.h"
#include "sdl/shared_object.h"

#include <m64p_api/m64p_common.h>
#include <m64p_api/m64p_plugin.h>

#define LOADFUNC(func) func = m_so.LoadFunction<ptr_##func>(#func)

namespace M64P {

inline void Checked(const char* function_name, m64p_error error_code)
{
    if (error_code != M64ERR_SUCCESS)
        throw Error{function_name, error_code};
}

class Plugin::Loader {
public:
    ptr_PluginGetVersion PluginGetVersion;
	ptr_PluginStartup PluginStartup;
	ptr_PluginShutdown PluginShutdown;

    void Load(const std::filesystem::path& path)
    {
        m_so = SDL::SharedObject{path};

        LOADFUNC(PluginGetVersion);
	    LOADFUNC(PluginStartup);
	    LOADFUNC(PluginShutdown);
    }

    void Unload()
    {
        m_so = {};
    }

    m64p_dynlib_handle GetHandle() const
    {
        return reinterpret_cast<m64p_dynlib_handle>(m_so.Get());
    }

private:
    SDL::SharedObject m_so;
};

Plugin::Plugin() :
    m_ld{std::make_unique<Loader>()}
{}

Plugin::~Plugin() = default;

void Plugin::Load(const std::filesystem::path& path)
{
    m_ld->Load(path);
}

void Plugin::Unload()
{
    m_ld->Unload();
}

m64p_dynlib_handle Plugin::GetHandle() const
{
    return m_ld->GetHandle();
}

m64p_plugin_type Plugin::GetType()
{
    m64p_plugin_type v;
	Checked("PluginGetVersion:GetType", m_ld->PluginGetVersion(&v, nullptr, nullptr, nullptr, nullptr));

	return v;
}

Version Plugin::GetVersion()
{
    int v;
	Checked("PluginGetVersion:GetVersion", m_ld->PluginGetVersion(nullptr, &v, nullptr, nullptr, nullptr));

    Version ver;
    ver.patch = v & 0xff;
    ver.minor = (v >> 8) & 0xff;
    ver.major = (v >> 16) & 0xff;

    return ver;
}

Version Plugin::GetAPIVersion()
{
    int v;
	Checked("PluginGetVersion:GetAPIVersion", m_ld->PluginGetVersion(nullptr, nullptr, &v, nullptr, nullptr));

    Version ver;
    ver.patch = v & 0xff;
    ver.minor = (v >> 8) & 0xff;
    ver.major = (v >> 16) & 0xff;

    return ver;
}

std::string Plugin::GetName()
{
    const char* v;
	Checked("PluginGetVersion:GetName", m_ld->PluginGetVersion(nullptr, nullptr, nullptr, &v, nullptr));

    return v;
}

m64p_core_caps Plugin::GetCaps()
{
    int v;
	Checked("PluginGetVersion:GetCapabilities", m_ld->PluginGetVersion(nullptr, nullptr, nullptr, nullptr, &v));

	return static_cast<m64p_core_caps>(v);
}

void Plugin::Startup(const Core& core, void* context, DebugCallback debug_cb)
{
    Checked("PluginStartup", m_ld->PluginStartup(core.GetHandle(), context, debug_cb));
}

void Plugin::Shutdown()
{
    Checked("PluginShutdown", m_ld->PluginShutdown());
}

}
