#include "m64p/core.h"
#include "m64p/error.h"
#include "m64p/plugin.h"
#include "sdl/shared_object.h"

#include <fmt/format.h>
#include <m64p_api/m64p_common.h>
#include <m64p_api/m64p_config.h>
#include <m64p_api/m64p_debugger.h>
#include <m64p_api/m64p_ext.h>
#include <m64p_api/m64p_frontend.h>
#include <m64p_api/m64p_vidext.h>

#define LOADFUNC(func) func = m_so.LoadFunction<ptr_##func>(#func)

namespace M64P {

inline void Checked(const char* function_name, m64p_error error_code)
{
    if (error_code != M64ERR_SUCCESS)
        throw Error{function_name, error_code};
}

class Core::Loader {
public:
    ptr_PluginGetVersion PluginGetVersion;
	ptr_CoreGetAPIVersions CoreGetAPIVersions;
	ptr_CoreErrorMessage CoreErrorMessage;

	ptr_CoreStartup CoreStartup;
	ptr_CoreShutdown CoreShutdown;
	ptr_CoreAttachPlugin CoreAttachPlugin;
	ptr_CoreDetachPlugin CoreDetachPlugin;
	ptr_CoreDoCommand CoreDoCommand;
	ptr_CoreOverrideVidExt CoreOverrideVidExt;
	ptr_CoreAddCheat CoreAddCheat;
	ptr_CoreCheatEnabled CoreCheatEnabled;
	ptr_CoreGetRomSettings CoreGetRomSettings;

	ptr_ConfigListSections ConfigListSections;
	ptr_ConfigOpenSection ConfigOpenSection;
	ptr_ConfigListParameters ConfigListParameters;
	ptr_ConfigSaveFile ConfigSaveFile;
	ptr_ConfigSaveSection ConfigSaveSection;
	ptr_ConfigHasUnsavedChanges ConfigHasUnsavedChanges;
	ptr_ConfigDeleteSection ConfigDeleteSection;
	ptr_ConfigRevertChanges ConfigRevertChanges;
	ptr_ConfigSetParameter ConfigSetParameter;
	ptr_ConfigSetParameterHelp ConfigSetParameterHelp;
	ptr_ConfigGetParameter ConfigGetParameter;
	ptr_ConfigGetParameterType ConfigGetParameterType;
	ptr_ConfigGetParameterHelp ConfigGetParameterHelp;
	ptr_ConfigSetDefaultInt ConfigSetDefaultInt;
	ptr_ConfigSetDefaultFloat ConfigSetDefaultFloat;
	ptr_ConfigSetDefaultBool ConfigSetDefaultBool;
	ptr_ConfigSetDefaultString ConfigSetDefaultString;
	ptr_ConfigGetParamInt ConfigGetParamInt;
	ptr_ConfigGetParamFloat ConfigGetParamFloat;
	ptr_ConfigGetParamBool ConfigGetParamBool;
	ptr_ConfigGetParamString ConfigGetParamString;
	ptr_ConfigGetSharedDataFilepath ConfigGetSharedDataFilepath;
	ptr_ConfigGetUserConfigPath ConfigGetUserConfigPath;
	ptr_ConfigGetUserDataPath ConfigGetUserDataPath;
	ptr_ConfigGetUserCachePath ConfigGetUserCachePath;

    ptr_DebugSetCallbacks DebugSetCallbacks;
    ptr_DebugSetRunState DebugSetRunState;
    ptr_DebugGetState DebugGetState;
    ptr_DebugStep DebugStep;
    ptr_DebugDecodeOp DebugDecodeOp;
    ptr_DebugMemGetRecompInfo DebugMemGetRecompInfo;
    ptr_DebugMemGetMemInfo DebugMemGetMemInfo;
    ptr_DebugMemGetPointer DebugMemGetPointer;
    ptr_DebugMemRead64 DebugMemRead64;
    ptr_DebugMemRead32 DebugMemRead32;
    ptr_DebugMemRead16 DebugMemRead16;
    ptr_DebugMemRead8 DebugMemRead8;
    ptr_DebugMemWrite64 DebugMemWrite64;
    ptr_DebugMemWrite32 DebugMemWrite32;
    ptr_DebugMemWrite16 DebugMemWrite16;
    ptr_DebugMemWrite8 DebugMemWrite8;
    ptr_DebugGetCPUDataPtr DebugGetCPUDataPtr;
    ptr_DebugBreakpointLookup DebugBreakpointLookup;
    ptr_DebugBreakpointCommand DebugBreakpointCommand;
    ptr_DebugBreakpointTriggeredBy DebugBreakpointTriggeredBy;
    ptr_DebugVirtualToPhysical DebugVirtualToPhysical;

    ptr_ExtSetFrameCallback ExtSetFrameCallback;
    ptr_ExtSetVICallback ExtSetVICallback;
    ptr_ExtSetResetCallback ExtSetResetCallback;
    ptr_ExtSetPauseLoopCallback ExtSetPauseLoopCallback;
    ptr_ExtSetCoreEventCallback ExtSetCoreEventCallback;
    ptr_ExtGetDRAMPtr ExtGetDRAMPtr;
    ptr_ExtGetDRAMSize ExtGetDRAMSize;
    ptr_ExtGetROMPtr ExtGetROMPtr;
    ptr_ExtGetROMSize ExtGetROMSize;
    ptr_ExtInvalidateCachedCode ExtInvalidateCachedCode;
    ptr_ExtInputKeyDown ExtInputKeyDown;
    ptr_ExtInputKeyUp ExtInputKeyUp;
    ptr_ExtGfxResizeOutput ExtGfxResizeOutput;
    ptr_ExtResetInputPlugin ExtResetInputPlugin;
    ptr_ExtContGetInput ExtContGetInput;
    ptr_ExtContSetInput ExtContSetInput;
    ptr_ExtContGetPluginFilter ExtContGetPluginFilter;
    ptr_ExtContSetPluginFilter ExtContSetPluginFilter;

    ptr_ExtRDRAMRead8 ExtRDRAMRead8;
    ptr_ExtRDRAMRead16 ExtRDRAMRead16;
    ptr_ExtRDRAMRead32 ExtRDRAMRead32;
    ptr_ExtRDRAMReadBuffer ExtRDRAMReadBuffer;
    ptr_ExtRDRAMWrite8 ExtRDRAMWrite8;
    ptr_ExtRDRAMWrite16 ExtRDRAMWrite16;
    ptr_ExtRDRAMWrite32 ExtRDRAMWrite32;
    ptr_ExtRDRAMWriteBuffer ExtRDRAMWriteBuffer;
    ptr_ExtROMRead8 ExtROMRead8;
    ptr_ExtROMRead16 ExtROMRead16;
    ptr_ExtROMRead32 ExtROMRead32;
    ptr_ExtROMReadBuffer ExtROMReadBuffer;
    ptr_ExtROMWrite8 ExtROMWrite8;
    ptr_ExtROMWrite16 ExtROMWrite16;
    ptr_ExtROMWrite32 ExtROMWrite32;
    ptr_ExtROMWriteBuffer ExtROMWriteBuffer;

    void Load(const std::filesystem::path& path)
    {
        m_so = SDL::SharedObject{path};

        LOADFUNC(PluginGetVersion);
        LOADFUNC(CoreGetAPIVersions);
        LOADFUNC(CoreErrorMessage);

        LOADFUNC(CoreStartup);
        LOADFUNC(CoreShutdown);
        LOADFUNC(CoreAttachPlugin);
        LOADFUNC(CoreDetachPlugin);
        LOADFUNC(CoreDoCommand);
        LOADFUNC(CoreOverrideVidExt);
        LOADFUNC(CoreAddCheat);
        LOADFUNC(CoreCheatEnabled);
        LOADFUNC(CoreGetRomSettings);

        LOADFUNC(ConfigListSections);
        LOADFUNC(ConfigOpenSection);
        LOADFUNC(ConfigListParameters);
        LOADFUNC(ConfigSaveFile);
        LOADFUNC(ConfigSaveSection);
        LOADFUNC(ConfigHasUnsavedChanges);
        LOADFUNC(ConfigDeleteSection);
        LOADFUNC(ConfigRevertChanges);
        LOADFUNC(ConfigSetParameter);
        LOADFUNC(ConfigSetParameterHelp);
        LOADFUNC(ConfigGetParameter);
        LOADFUNC(ConfigGetParameterType);
        LOADFUNC(ConfigGetParameterHelp);
        LOADFUNC(ConfigSetDefaultInt);
        LOADFUNC(ConfigSetDefaultFloat);
        LOADFUNC(ConfigSetDefaultBool);
        LOADFUNC(ConfigSetDefaultString);
        LOADFUNC(ConfigGetParamInt);
        LOADFUNC(ConfigGetParamFloat);
        LOADFUNC(ConfigGetParamBool);
        LOADFUNC(ConfigGetParamString);
        LOADFUNC(ConfigGetSharedDataFilepath);
        LOADFUNC(ConfigGetUserConfigPath);
        LOADFUNC(ConfigGetUserDataPath);
        LOADFUNC(ConfigGetUserCachePath);

        LOADFUNC(DebugSetCallbacks);
        LOADFUNC(DebugSetRunState);
        LOADFUNC(DebugGetState);
        LOADFUNC(DebugStep);
        LOADFUNC(DebugDecodeOp);
        LOADFUNC(DebugMemGetRecompInfo);
        LOADFUNC(DebugMemGetMemInfo);
        LOADFUNC(DebugMemGetPointer);
        LOADFUNC(DebugMemRead64);
        LOADFUNC(DebugMemRead32);
        LOADFUNC(DebugMemRead16);
        LOADFUNC(DebugMemRead8);
        LOADFUNC(DebugMemWrite64);
        LOADFUNC(DebugMemWrite32);
        LOADFUNC(DebugMemWrite16);
        LOADFUNC(DebugMemWrite8);
        LOADFUNC(DebugGetCPUDataPtr);
        LOADFUNC(DebugBreakpointLookup);
        LOADFUNC(DebugBreakpointCommand);
        LOADFUNC(DebugBreakpointTriggeredBy);
        LOADFUNC(DebugVirtualToPhysical);

        LOADFUNC(ExtSetFrameCallback);
        LOADFUNC(ExtSetVICallback);
        LOADFUNC(ExtSetResetCallback);
        LOADFUNC(ExtSetPauseLoopCallback);
        LOADFUNC(ExtSetCoreEventCallback);
        LOADFUNC(ExtGetDRAMPtr);
        LOADFUNC(ExtGetDRAMSize);
        LOADFUNC(ExtGetROMPtr);
        LOADFUNC(ExtGetROMSize);
        LOADFUNC(ExtInvalidateCachedCode);
        LOADFUNC(ExtInputKeyDown);
        LOADFUNC(ExtInputKeyUp);
        LOADFUNC(ExtGfxResizeOutput);
        LOADFUNC(ExtResetInputPlugin);
        LOADFUNC(ExtContGetInput);
        LOADFUNC(ExtContSetInput);
        LOADFUNC(ExtContGetPluginFilter);
        LOADFUNC(ExtContSetPluginFilter);

        LOADFUNC(ExtRDRAMRead8);
        LOADFUNC(ExtRDRAMRead16);
        LOADFUNC(ExtRDRAMRead32);
        LOADFUNC(ExtRDRAMReadBuffer);
        LOADFUNC(ExtRDRAMWrite8);
        LOADFUNC(ExtRDRAMWrite16);
        LOADFUNC(ExtRDRAMWrite32);
        LOADFUNC(ExtRDRAMWriteBuffer);
        LOADFUNC(ExtROMRead8);
        LOADFUNC(ExtROMRead16);
        LOADFUNC(ExtROMRead32);
        LOADFUNC(ExtROMReadBuffer);
        LOADFUNC(ExtROMWrite8);
        LOADFUNC(ExtROMWrite16);
        LOADFUNC(ExtROMWrite32);
        LOADFUNC(ExtROMWriteBuffer);
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

Core::Core() :
    m_ld{std::make_unique<Loader>()}
{}

Core::~Core() = default;

void Core::Load(const std::filesystem::path& path)
{
    m_ld->Load(path);
}

void Core::Unload()
{
    m_ld->Unload();
}

m64p_dynlib_handle Core::GetHandle() const
{
    return m_ld->GetHandle();
}

m64p_plugin_type Core::GetType()
{
    m64p_plugin_type v;
	Checked("PluginGetVersion:GetType", m_ld->PluginGetVersion(&v, nullptr, nullptr, nullptr, nullptr));

	return v;
}

Version Core::GetVersion()
{
    int v;
	Checked("PluginGetVersion:GetVersion", m_ld->PluginGetVersion(nullptr, &v, nullptr, nullptr, nullptr));

    Version ver;
    ver.patch = v & 0xff;
    ver.minor = (v >> 8) & 0xff;
    ver.major = (v >> 16) & 0xff;

    return ver;
}

Version Core::GetAPIVersion()
{
    int v;
	Checked("PluginGetVersion:GetAPIVersion", m_ld->PluginGetVersion(nullptr, nullptr, &v, nullptr, nullptr));

    Version ver;
    ver.patch = v & 0xff;
    ver.minor = (v >> 8) & 0xff;
    ver.major = (v >> 16) & 0xff;

    return ver;
}

std::string Core::GetName()
{
    const char* v;
	Checked("PluginGetVersion:GetName", m_ld->PluginGetVersion(nullptr, nullptr, nullptr, &v, nullptr));

    return v;
}

m64p_core_caps Core::GetCaps()
{
    int v;
	Checked("PluginGetVersion:GetCapabilities", m_ld->PluginGetVersion(nullptr, nullptr, nullptr, nullptr, &v));

	return static_cast<m64p_core_caps>(v);
}

Version Core::GetConfigVersion()
{
    int v;
    Checked("CoreGetAPIVersions:GetConfigVersion", m_ld->CoreGetAPIVersions(&v, nullptr, nullptr, nullptr));

    Version ver;
    ver.patch = v & 0xff;
    ver.minor = (v >> 8) & 0xff;
    ver.major = (v >> 16) & 0xff;

    return ver;
}

Version Core::GetDebugVersion()
{
    int v;
    Checked("CoreGetAPIVersions:GetDebugVersion", m_ld->CoreGetAPIVersions(nullptr, &v, nullptr, nullptr));

    Version ver;
    ver.patch = v & 0xff;
    ver.minor = (v >> 8) & 0xff;
    ver.major = (v >> 16) & 0xff;

    return ver;
}

Version Core::GetVidExtVersion()
{
    int v;
    Checked("CoreGetAPIVersions:GetVidExtVersion", m_ld->CoreGetAPIVersions(nullptr, nullptr, &v, nullptr));

    Version ver;
    ver.patch = v & 0xff;
    ver.minor = (v >> 8) & 0xff;
    ver.major = (v >> 16) & 0xff;

    return ver;
}

Version Core::GetExtraVersion()
{
    int v;
    Checked("CoreGetAPIVersions:GetExtraVersion", m_ld->CoreGetAPIVersions(nullptr, nullptr, nullptr, &v));

    Version ver;
    ver.patch = v & 0xff;
    ver.minor = (v >> 8) & 0xff;
    ver.major = (v >> 16) & 0xff;

    return ver;
}

void Core::Startup(int api_version, const std::filesystem::path& config_path, const std::filesystem::path& data_path,
    void* context, DebugCallback debug_cb, void* context2, StateCallback state_cb)
{
    Checked("CoreStartup",
        m_ld->CoreStartup(api_version,
            config_path.string().c_str(),
            data_path.string().c_str(),
            context, debug_cb,
            context2, state_cb)
        );
}

void Core::Shutdown()
{
    Checked("CoreShutdown", m_ld->CoreShutdown());
}

void Core::AttachPlugin(Plugin& plugin)
{
    Checked("CoreAttachPlugin", m_ld->CoreAttachPlugin(plugin.GetType(), plugin.GetHandle()));
}

void Core::DetachPlugin(Plugin& plugin)
{
    Checked("CoreDetachPlugin", m_ld->CoreDetachPlugin(plugin.GetType()));
}

void Core::OverrideVidExt(m64p_video_extension_functions& functions)
{
    Checked("CoreOverrideVidExt", m_ld->CoreOverrideVidExt(&functions));
}

void Core::AddCheat(const std::string& name, m64p_cheat_code* codes, std::size_t num_codes)
{
    Checked("CoreAddCheat", m_ld->CoreAddCheat(name.c_str(), codes, static_cast<int>(num_codes)));
}

void Core::SetCheatEnabled(const std::string& name, bool enabled)
{
    Checked("CoreCheatEnabled", m_ld->CoreCheatEnabled(name.c_str(), enabled));
}

void Core::GetROMSettings(m64p_rom_settings* settings, int crc1, int crc2)
{
    Checked("CoreGetRomSettings", m_ld->CoreGetRomSettings(settings, sizeof(m64p_rom_settings), crc1, crc2));
}

void Core::OpenROM(const u8* data, std::size_t len)
{
    Checked("CoreDoCommand:OpenRom", m_ld->CoreDoCommand(M64CMD_ROM_OPEN, static_cast<int>(len), const_cast<u8*>(data)));
}

void Core::CloseROM()
{
    Checked("CoreDoCommand:CloseRom", m_ld->CoreDoCommand(M64CMD_ROM_CLOSE, 0, nullptr));
}

void Core::GetROMHeader(m64p_rom_header* header)
{
    Checked("CoreDoCommand:GetRomHeader", m_ld->CoreDoCommand(M64CMD_ROM_GET_HEADER, sizeof(m64p_rom_header), header));
}

void Core::GetROMSettings(m64p_rom_settings* settings)
{
    Checked("CoreDoCommand:GetRomSettings", m_ld->CoreDoCommand(M64CMD_ROM_GET_SETTINGS, sizeof(m64p_rom_settings), settings));
}

void Core::Execute()
{
    Checked("CoreDoCommand:Execute", m_ld->CoreDoCommand(M64CMD_EXECUTE, 0, nullptr));
}

void Core::Stop()
{
    Checked("CoreDoCommand:Stop", m_ld->CoreDoCommand(M64CMD_STOP, 0, nullptr));
}

void Core::Pause()
{
    Checked("CoreDoCommand:Pause", m_ld->CoreDoCommand(M64CMD_PAUSE, 0, nullptr));
}

void Core::Resume()
{
    Checked("CoreDoCommand:Resume", m_ld->CoreDoCommand(M64CMD_RESUME, 0, nullptr));
}

void Core::SetStateSlot(int slot)
{
    Checked("CoreDoCommand:SetStateSlot", m_ld->CoreDoCommand(M64CMD_STATE_SET_SLOT, std::clamp(slot, 0, 9), nullptr));
}

void Core::LoadState()
{
    Checked("CoreDoCommand:LoadStateFromSlot", m_ld->CoreDoCommand(M64CMD_STATE_LOAD, 0, nullptr));
}

void Core::LoadState(int slot)
{
    auto prev = GetStateSlot();
    SetStateSlot(slot);
    LoadState();
    SetStateSlot(prev);
}

void Core::LoadState(const std::filesystem::path& path)
{
    Checked("CoreDoCommand:LoadStateFromFile", m_ld->CoreDoCommand(M64CMD_STATE_LOAD, 0, const_cast<char*>(path.string().c_str())));
}

void Core::SaveState()
{
    Checked("CoreDoCommand:SaveStateToSlot", m_ld->CoreDoCommand(M64CMD_STATE_SAVE, 0, nullptr));
}

void Core::SaveState(int slot)
{
    auto prev = GetStateSlot();
    SetStateSlot(slot);
    SaveState();
    SetStateSlot(prev);
}

void Core::SaveState(const std::filesystem::path& path)
{
    Checked("CoreDoCommand:SaveStateToFile", m_ld->CoreDoCommand(M64CMD_STATE_SAVE, 1, const_cast<char*>(path.string().c_str())));
}

void Core::TakeNextScreenshot()
{
    Checked("CoreDoCommand:TakeNextScreenshot", m_ld->CoreDoCommand(M64CMD_TAKE_NEXT_SCREENSHOT, 0, nullptr));
}

void Core::ReadScreen(int buffer_index, u8* buffer_data)
{
    Checked("CoreDoCommand:ReadScreen", m_ld->CoreDoCommand(M64CMD_READ_SCREEN, static_cast<int>(buffer_index), buffer_data));
}

void Core::Reset(bool do_hard_reset)
{
    Checked("CoreDoCommand:Reset", m_ld->CoreDoCommand(M64CMD_RESET, do_hard_reset, nullptr));
}

void Core::AdvanceFrame()
{
    Checked("CoreDoCommand:AdvanceFrame", m_ld->CoreDoCommand(M64CMD_ADVANCE_FRAME, 0, nullptr));
}

m64p_emu_state Core::GetEmuState()
{
    int v;
    Checked("CoreDoCommand:GetEmuState", m_ld->CoreDoCommand(M64CMD_CORE_STATE_QUERY, M64CORE_EMU_STATE, &v));

    return static_cast<m64p_emu_state>(v);
}

void Core::SetEmuState(m64p_emu_state state)
{
    Checked("CoreDoCommand:SetEmuState", m_ld->CoreDoCommand(M64CMD_CORE_STATE_SET, M64CORE_EMU_STATE, &state));
}

m64p_video_mode Core::GetVideoMode()
{
    int v;
    Checked("CoreDoCommand:GetVideoMode", m_ld->CoreDoCommand(M64CMD_CORE_STATE_QUERY, M64CORE_VIDEO_MODE, &v));

    return static_cast<m64p_video_mode>(v);
}

void Core::SetVideoMode(m64p_video_mode mode)
{
    Checked("CoreDoCommand:SetVideoMode", m_ld->CoreDoCommand(M64CMD_CORE_STATE_SET, M64CORE_VIDEO_MODE, &mode));
}

int Core::GetStateSlot()
{
    int v;
    Checked("CoreDoCommand:GetStateSlot", m_ld->CoreDoCommand(M64CMD_CORE_STATE_QUERY, M64CORE_SAVESTATE_SLOT, &v));

    return v;
}

int Core::GetSpeedFactor()
{
    int v;
    Checked("CoreDoCommand:GetSpeedFactor", m_ld->CoreDoCommand(M64CMD_CORE_STATE_QUERY, M64CORE_SPEED_FACTOR, &v));

    return v;
}

void Core::SetSpeedFactor(int factor)
{
    factor = std::clamp(factor, 0, 1000);
    Checked("CoreDoCommand:SetSpeedFactor", m_ld->CoreDoCommand(M64CMD_CORE_STATE_SET, M64CORE_SPEED_FACTOR, &factor));
}

bool Core::IsSpeedLimiterEnabled()
{
    int v;
    Checked("CoreDoCommand:IsSpeedLimiterEnabled", m_ld->CoreDoCommand(M64CMD_CORE_STATE_QUERY, M64CORE_SPEED_LIMITER, &v));

    return v;
}

void Core::SetSpeedLimiterEnabled(bool enabled)
{
    int v = enabled;
    Checked("CoreDoCommand:SetSpeedLimiterEnabled", m_ld->CoreDoCommand(M64CMD_CORE_STATE_SET, M64CORE_SPEED_LIMITER, &v));
}

m64p_2d_size Core::GetVideoSize()
{
    int v;
    Checked("CoreDoCommand:GetVideoSize", m_ld->CoreDoCommand(M64CMD_CORE_STATE_QUERY, M64CORE_VIDEO_SIZE, &v));

    m64p_2d_size size;
    size.uiWidth = (v >> 16) & 0xffff;
    size.uiHeight = v & 0xffff;

    return size;
}

void Core::SetVideoSize(const m64p_2d_size& size)
{
    int v = ((size.uiWidth & 0xffff) << 16) | (size.uiHeight & 0xffff);
    Checked("CoreDoCommand:SetVideoSize", m_ld->CoreDoCommand(M64CMD_CORE_STATE_SET, M64CORE_VIDEO_SIZE, &v));
}

int Core::GetAudioVolume()
{
    int v;
    Checked("CoreDoCommand:GetAudioVolume", m_ld->CoreDoCommand(M64CMD_CORE_STATE_QUERY, M64CORE_AUDIO_VOLUME, &v));

    return v;
}

void Core::SetAudioVolume(int volume)
{
    volume = std::clamp(volume, 0, 100);
    Checked("CoreDoCommand:SetAudioVolume", m_ld->CoreDoCommand(M64CMD_CORE_STATE_SET, M64CORE_AUDIO_VOLUME, &volume));
}

bool Core::IsAudioMuted()
{
    int v;
    Checked("CoreDoCommand:IsAudioMuted", m_ld->CoreDoCommand(M64CMD_CORE_STATE_QUERY, M64CORE_AUDIO_MUTE, &v));

    return v;
}

void Core::SetAudioMuted(bool muted)
{
    int v = muted;
    Checked("CoreDoCommand:SetAudioMuted", m_ld->CoreDoCommand(M64CMD_CORE_STATE_SET, M64CORE_AUDIO_MUTE, &v));
}

bool Core::IsGSButtonPressed()
{
    int v;
    Checked("CoreDoCommand:IsGsButtonPressed", m_ld->CoreDoCommand(M64CMD_CORE_STATE_QUERY, M64CORE_INPUT_GAMESHARK, &v));

    return v;
}

void Core::SetGSButtonPressed(bool pressed)
{
    int v = pressed;
    Checked("CoreDoCommand:SetGsButtonPressed", m_ld->CoreDoCommand(M64CMD_CORE_STATE_SET, M64CORE_INPUT_GAMESHARK, &v));
}

void Core::ConfigSaveFile()
{
    Checked("ConfigSaveFile", m_ld->ConfigSaveFile());
}

bool Core::ConfigHasUnsavedChanges()
{
    return m_ld->ConfigHasUnsavedChanges(nullptr);
}

std::vector<std::string> Core::ConfigListSections()
{
    std::vector<std::string> v;
    Checked("ConfigListSections", m_ld->ConfigListSections(&v, [](void* context, const char* section_name) {
        static_cast<std::vector<std::string>*>(context)->push_back(section_name);
    }));

    return v;
}

Core::ConfigSection::ConfigSection(Core& core, const std::string& name, m64p_handle handle) :
    m_core{&core},
    m_name{name},
    m_handle{handle}
{}

std::string Core::ConfigSection::GetName()
{
    return m_name;
}

std::vector<Core::ConfigSection::Param> Core::ConfigSection::ListParams()
{
    std::vector<Param> v;
    Checked("ConfigListParameters", m_core->m_ld->ConfigListParameters(m_handle, &v,
        [](void* context, const char* param_name, m64p_type param_type) {
            static_cast<std::vector<Param>*>(context)->push_back({param_name, param_type});
        }));

    return v;
}

void Core::ConfigSection::Save()
{
    Checked("ConfigSaveSection", m_core->m_ld->ConfigSaveSection(m_name.c_str()));
}

bool Core::ConfigSection::HasUnsavedChanges()
{
    return m_core->m_ld->ConfigHasUnsavedChanges(m_name.c_str());
}

void Core::ConfigSection::Erase()
{
    Checked("ConfigDeleteSection", m_core->m_ld->ConfigDeleteSection(m_name.c_str()));
}

void Core::ConfigSection::RevertChanges()
{
    Checked("ConfigRevertChanges", m_core->m_ld->ConfigRevertChanges(m_name.c_str()));
}

std::string Core::ConfigSection::GetHelp(const std::string& name)
{
    return m_core->m_ld->ConfigGetParameterHelp(m_handle, name.c_str());
}

void Core::ConfigSection::SetHelp(const std::string& name, const std::string& help)
{
    Checked("ConfigSetParameterHelp", m_core->m_ld->ConfigSetParameterHelp(m_handle, name.c_str(), help.c_str()));
}

m64p_type Core::ConfigSection::GetType(const std::string& name)
{
    m64p_type v;
    Checked("ConfigGetParameterType", m_core->m_ld->ConfigGetParameterType(m_handle, name.c_str(), &v));

    return v;
}

void Core::ConfigSection::SetDefaultInt(const std::string& name, int value, const std::string& help)
{
    Checked("ConfigSetDefaultInt", m_core->m_ld->ConfigSetDefaultInt(m_handle, name.c_str(), value, help.c_str()));
}

void Core::ConfigSection::SetDefaultFloat(const std::string& name, float value, const std::string& help)
{
    Checked("ConfigSetDefaultFloat", m_core->m_ld->ConfigSetDefaultFloat(m_handle, name.c_str(), value, help.c_str()));
}

void Core::ConfigSection::SetDefaultBool(const std::string& name, bool value, const std::string& help)
{
    Checked("ConfigSetDefaultBool", m_core->m_ld->ConfigSetDefaultBool(m_handle, name.c_str(), value, help.c_str()));
}

void Core::ConfigSection::SetDefaultString(const std::string& name, const std::string& value, const std::string& help)
{
    Checked("ConfigSetDefaultString", m_core->m_ld->ConfigSetDefaultString(m_handle, name.c_str(), value.c_str(), help.c_str()));
}

int Core::ConfigSection::GetInt(const std::string& name)
{
    return m_core->m_ld->ConfigGetParamInt(m_handle, name.c_str());
}

int Core::ConfigSection::GetIntOr(const std::string& name, int value)
{
    int v;
    m64p_error ret = m_core->m_ld->ConfigGetParameter(m_handle, name.c_str(), M64TYPE_INT, &v, static_cast<int>(sizeof(int)));

    return ret == M64ERR_SUCCESS ? v : value;
}

void Core::ConfigSection::SetInt(const std::string& name, int value)
{
    Checked("ConfigSetParameter:SetInt", m_core->m_ld->ConfigSetParameter(m_handle, name.c_str(), M64TYPE_INT, &value));
}

float Core::ConfigSection::GetFloat(const std::string& name)
{
    return m_core->m_ld->ConfigGetParamFloat(m_handle, name.c_str());
}

float Core::ConfigSection::GetFloatOr(const std::string& name, float value)
{
    float v;
    m64p_error ret = m_core->m_ld->ConfigGetParameter(m_handle, name.c_str(), M64TYPE_FLOAT, &v, static_cast<int>(sizeof(float)));

    return ret == M64ERR_SUCCESS ? v : value;
}

void Core::ConfigSection::SetFloat(const std::string& name, float value)
{
    Checked("ConfigSetParameter:SetFloat", m_core->m_ld->ConfigSetParameter(m_handle, name.c_str(), M64TYPE_FLOAT, &value));
}

bool Core::ConfigSection::GetBool(const std::string& name)
{
    return m_core->m_ld->ConfigGetParamBool(m_handle, name.c_str());
}

bool Core::ConfigSection::GetBoolOr(const std::string& name, bool value)
{
    int v;
    m64p_error ret = m_core->m_ld->ConfigGetParameter(m_handle, name.c_str(), M64TYPE_BOOL, &v, static_cast<int>(sizeof(int)));

    return ret == M64ERR_SUCCESS ? v : value;
}

void Core::ConfigSection::SetBool(const std::string& name, bool value)
{
    int v = value;
    Checked("ConfigSetParameter:SetBool", m_core->m_ld->ConfigSetParameter(m_handle, name.c_str(), M64TYPE_BOOL, &v));
}

std::string Core::ConfigSection::GetString(const std::string& name)
{
    return m_core->m_ld->ConfigGetParamString(m_handle, name.c_str());
}

std::string Core::ConfigSection::GetStringOr(const std::string& name, const std::string& value)
{
    char v[200];
    m64p_error ret = m_core->m_ld->ConfigGetParameter(m_handle, name.c_str(), M64TYPE_STRING, v, 200);

    return ret == M64ERR_SUCCESS ? v : value;
}

void Core::ConfigSection::SetString(const std::string& name, const std::string& value)
{
    Checked("ConfigSetParameter:SetString", m_core->m_ld->ConfigSetParameter(m_handle, name.c_str(), M64TYPE_STRING, const_cast<char*>(value.c_str())));
}

Core::ConfigSection Core::ConfigOpenSection(const std::string& name)
{
    m64p_handle handle;
    Checked("ConfigOpenSection", m_ld->ConfigOpenSection(name.c_str(), &handle));

    return {*this, name, handle};
}

std::filesystem::path Core::GetSharedDataFilePath(const std::string& file)
{
    auto v = m_ld->ConfigGetSharedDataFilepath(file.c_str());
    return v ? v : "";
}

std::filesystem::path Core::GetUserConfigPath()
{
    return m_ld->ConfigGetUserConfigPath();
}

std::filesystem::path Core::GetUserDataPath()
{
    return m_ld->ConfigGetUserDataPath();
}

std::filesystem::path Core::GetUserCachePath()
{
    return m_ld->ConfigGetUserCachePath();
}

void Core::DebugSetCallbacks(DebugInitCallback init_cb, DebugUpdateCallback update_cb, DebugVICallback vi_cb)
{
    Checked("DebugSetCallbacks", m_ld->DebugSetCallbacks(init_cb, update_cb, vi_cb));
}

m64p_dbg_runstate Core::DebugGetRunState()
{
    return static_cast<m64p_dbg_runstate>(m_ld->DebugGetState(M64P_DBG_RUN_STATE));
}

void Core::DebugSetRunState(m64p_dbg_runstate state)
{
    Checked("DebugSetRunState", m_ld->DebugSetRunState(state));
}

void Core::DebugStep()
{
    Checked("DebugStep", m_ld->DebugStep());
}

std::string Core::DebugDecodeOp(u32 inst, int pc)
{
    char op[100] = "", args[100] = "";
    m_ld->DebugDecodeOp(inst, op, args, pc);

    return fmt::format("{} {}", op, args);
}

u64 Core::DebugMemRead64(u32 addr)
{
    return m_ld->DebugMemRead64(addr);
}

u32 Core::DebugMemRead32(u32 addr)
{
    return m_ld->DebugMemRead32(addr);
}

u16 Core::DebugMemRead16(u32 addr)
{
    return m_ld->DebugMemRead16(addr);
}

u8 Core::DebugMemRead8(u32 addr)
{
    return m_ld->DebugMemRead8(addr);
}

void Core::DebugMemWrite64(u32 addr, u64 val)
{
    m_ld->DebugMemWrite64(addr, val);
}

void Core::DebugMemWrite32(u32 addr, u32 val)
{
    m_ld->DebugMemWrite32(addr, val);
}

void Core::DebugMemWrite16(u32 addr, u16 val)
{
    m_ld->DebugMemWrite16(addr, val);
}

void Core::DebugMemWrite8(u32 addr, u8 val)
{
    m_ld->DebugMemWrite8(addr, val);
}

int Core::DebugBpCount()
{
    return m_ld->DebugGetState(M64P_DBG_NUM_BREAKPOINTS);
}

int Core::DebugBpLookup(u32 addr, u32 size, u32 flags)
{
    return m_ld->DebugBreakpointLookup(addr, size, flags);
}

int Core::DebugBpAddAddr(u32 addr)
{
    return m_ld->DebugBreakpointCommand(M64P_BKP_CMD_ADD_ADDR, addr, nullptr);
}

int Core::DebugBpAddStruct(const m64p_breakpoint& bp)
{
    return m_ld->DebugBreakpointCommand(M64P_BKP_CMD_ADD_STRUCT, 0, const_cast<m64p_breakpoint*>(&bp));
}

void Core::DebugBpReplace(int index, const m64p_breakpoint& bp)
{
    m_ld->DebugBreakpointCommand(M64P_BKP_CMD_REPLACE, index, const_cast<m64p_breakpoint*>(&bp));
}

void Core::DebugBpRemoveAddr(u32 addr)
{
    m_ld->DebugBreakpointCommand(M64P_BKP_CMD_REMOVE_ADDR, addr, nullptr);
}

void Core::DebugBpRemoveIdx(int index)
{
    m_ld->DebugBreakpointCommand(M64P_BKP_CMD_REMOVE_IDX, index, nullptr);
}

void Core::DebugBpEnable(int index)
{
    m_ld->DebugBreakpointCommand(M64P_BKP_CMD_ENABLE, index, nullptr);
}

void Core::DebugBpDisable(int index)
{
    m_ld->DebugBreakpointCommand(M64P_BKP_CMD_DISABLE, index, nullptr);
}

int Core::DebugBpCheck(u32 addr)
{
    return m_ld->DebugBreakpointCommand(M64P_BKP_CMD_CHECK, addr, nullptr);
}

void Core::DebugBpTriggeredBy(u32* flags, u32* accessed)
{
    m_ld->DebugBreakpointTriggeredBy(flags, accessed);
}

int Core::DebugGetPrevPC()
{
    return m_ld->DebugGetState(M64P_DBG_PREVIOUS_PC);
}

int* Core::DebugGetPCPtr()
{
    return static_cast<int*>(m_ld->DebugGetCPUDataPtr(M64P_CPU_PC));
}

u32* Core::DebugGetRegPtr()
{
    return static_cast<u32*>(m_ld->DebugGetCPUDataPtr(M64P_CPU_REG_REG));
}

u32 Core::DebugVirtualToPhysical(u32 addr)
{
    return m_ld->DebugVirtualToPhysical(addr);
}

m64p_dbg_mem_type Core::DebugGetMemType(u32 addr)
{
    return static_cast<m64p_dbg_mem_type>(m_ld->DebugMemGetMemInfo(M64P_DBG_MEM_TYPE, addr));
}

m64p_dbg_mem_flags Core::DebugGetMemFlags(u32 addr)
{
    return static_cast<m64p_dbg_mem_flags>(m_ld->DebugMemGetMemInfo(M64P_DBG_MEM_FLAGS, addr));
}

void Core::SetFrameCallback(ExtCallback frame)
{
    m_ld->ExtSetFrameCallback(frame);
}

void Core::SetVICallback(ExtCallback vi)
{
    m_ld->ExtSetVICallback(vi);
}

void Core::SetResetCallback(ExtIntCallback reset)
{
    m_ld->ExtSetResetCallback(reset);
}

void Core::SetPauseLoopCallback(ExtCallback pause)
{
    m_ld->ExtSetPauseLoopCallback(pause);
}

void Core::SetEventCallback(ExtIntCallback event)
{
    m_ld->ExtSetCoreEventCallback(event);
}

u8* Core::GetDRAMPtr()
{
    return m_ld->ExtGetDRAMPtr();
}

std::size_t Core::GetDRAMSize()
{
    return m_ld->ExtGetDRAMSize();
}

u8* Core::GetROMPtr()
{
    return m_ld->ExtGetROMPtr();
}

std::size_t Core::GetROMSize()
{
    return m_ld->ExtGetROMSize();
}

void Core::InvalidateCachedCode()
{
    m_ld->ExtInvalidateCachedCode();
}

void Core::InputKeyDown(int mod, int sym)
{
    m_ld->ExtInputKeyDown(mod, sym);
}

void Core::InputKeyUp(int mod, int sym)
{
    m_ld->ExtInputKeyUp(mod, sym);
}

void Core::GfxResizeOutput(int w, int h)
{
    m_ld->ExtGfxResizeOutput(w, h);
}

void Core::ResetInputPlugin()
{
    m_ld->ExtResetInputPlugin();
}

u32 Core::ContGetInput(int index)
{
    return m_ld->ExtContGetInput(index);
}

void Core::ContSetInput(int index, u32 input)
{
    m_ld->ExtContSetInput(index, input);
}

u32 Core::ContGetPluginFilter(int index)
{
    return m_ld->ExtContGetPluginFilter(index);
}

void Core::ContSetPluginFilter(int index, u32 filter)
{
    m_ld->ExtContSetPluginFilter(index, filter);
}

u8 Core::RDRAMRead8(u32 addr)
{
    return m_ld->ExtRDRAMRead8(addr);
}

u16 Core::RDRAMRead16(u32 addr)
{
    return m_ld->ExtRDRAMRead16(addr);
}

u32 Core::RDRAMRead32(u32 addr)
{
    return m_ld->ExtRDRAMRead32(addr);
}

u8* Core::RDRAMReadBuffer(u32 addr, std::size_t len)
{
    return m_ld->ExtRDRAMReadBuffer(addr, len);
}

void Core::RDRAMWrite8(u32 addr, u8 val)
{
    m_ld->ExtRDRAMWrite8(addr, val);
}

void Core::RDRAMWrite16(u32 addr, u16 val)
{
    m_ld->ExtRDRAMWrite16(addr, val);
}

void Core::RDRAMWrite32(u32 addr, u32 val)
{
    m_ld->ExtRDRAMWrite32(addr, val);
}

void Core::RDRAMWriteBuffer(u32 addr, u8* buf, std::size_t len)
{
    m_ld->ExtRDRAMWriteBuffer(addr, buf, len);
}

u8 Core::ROMRead8(u32 addr)
{
    return m_ld->ExtROMRead8(addr);
}

u16 Core::ROMRead16(u32 addr)
{
    return m_ld->ExtROMRead16(addr);
}

u32 Core::ROMRead32(u32 addr)
{
    return m_ld->ExtROMRead32(addr);
}

u8* Core::ROMReadBuffer(u32 addr, std::size_t len)
{
    return m_ld->ExtROMReadBuffer(addr, len);
}

void Core::ROMWrite8(u32 addr, u8 val)
{
    m_ld->ExtROMWrite8(addr, val);
}

void Core::ROMWrite16(u32 addr, u16 val)
{
    m_ld->ExtROMWrite16(addr, val);
}

void Core::ROMWrite32(u32 addr, u32 val)
{
    m_ld->ExtROMWrite32(addr, val);
}

void Core::ROMWriteBuffer(u32 addr, u8* buf, std::size_t len)
{
    m_ld->ExtROMWriteBuffer(addr, buf, len);
}

}
