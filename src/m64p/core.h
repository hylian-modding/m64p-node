#pragma once

#include "common/types.h"
#include "m64p/version.h"

#include <filesystem>
#include <string>
#include <vector>

#include <m64p_api/m64p_ext.h>
#include <m64p_api/m64p_types.h>

namespace M64P {

class Plugin;

class Core {
public:
    using DebugCallback = void(*)(void* context, int level, const char* message);
    using StateCallback = void(*)(void* context, m64p_core_param param_type, int new_value);

    Core();
    ~Core();

    void Load(const std::filesystem::path& path);
    void Unload();
    m64p_dynlib_handle GetHandle() const;

    m64p_plugin_type GetType();
    Version GetVersion();
    Version GetAPIVersion();
    std::string GetName();
    m64p_core_caps GetCaps();

    Version GetConfigVersion();
    Version GetDebugVersion();
    Version GetVidExtVersion();
    Version GetExtraVersion();

    void Startup(int api_version, const std::filesystem::path& config_path, const std::filesystem::path& data_path,
        void* context, DebugCallback debug_cb, void* context2, StateCallback state_cb);
    void Shutdown();
    void AttachPlugin(Plugin& plugin);
    void DetachPlugin(Plugin& plugin);
    void OverrideVidExt(m64p_video_extension_functions& functions);
    void AddCheat(const std::string& name, m64p_cheat_code* codes, std::size_t num_codes);
    void SetCheatEnabled(const std::string& name, bool enabled);
    void GetROMSettings(m64p_rom_settings* settings, int crc1, int crc2);

    void OpenROM(const u8* data, std::size_t len);
    void CloseROM();
    void GetROMHeader(m64p_rom_header* header);
    void GetROMSettings(m64p_rom_settings* settings);
    void Execute();
    void Stop();
    void Pause();
    void Resume();
    void SetStateSlot(int slot);
    void LoadState();
    void LoadState(int slot);
    void LoadState(const std::filesystem::path& path);
    void SaveState();
    void SaveState(int slot);
    void SaveState(const std::filesystem::path& path);
    void TakeNextScreenshot();
    void ReadScreen(int buffer_index, u8* buffer_data);
    void Reset(bool do_hard_reset);
    void AdvanceFrame();

    m64p_emu_state GetEmuState();
    void SetEmuState(m64p_emu_state state);
    m64p_video_mode GetVideoMode();
    void SetVideoMode(m64p_video_mode mode);
    int GetStateSlot();
    int GetSpeedFactor();
    void SetSpeedFactor(int factor);
    bool IsSpeedLimiterEnabled();
    void SetSpeedLimiterEnabled(bool enabled);
    m64p_2d_size GetVideoSize();
    void SetVideoSize(const m64p_2d_size& size);
    int GetAudioVolume();
    void SetAudioVolume(int volume);
    bool IsAudioMuted();
    void SetAudioMuted(bool muted);
    bool IsGSButtonPressed();
    void SetGSButtonPressed(bool pressed);

    void ConfigSaveFile();
    bool ConfigHasUnsavedChanges();
    std::vector<std::string> ConfigListSections();

    class ConfigSection {
        friend Core;

    public:
        struct Param {
            std::string name;
            m64p_type type;
        };

        std::string GetName();
        std::vector<Param> ListParams();
        void Save();
        bool HasUnsavedChanges();
        void Erase();
        void RevertChanges();

        std::string GetHelp(const std::string& name);
        void SetHelp(const std::string& name, const std::string& help);
        m64p_type GetType(const std::string& name);

        void SetDefaultInt(const std::string& name, int value, const std::string& help = "");
        void SetDefaultFloat(const std::string& name, float value, const std::string& help = "");
        void SetDefaultBool(const std::string& name, bool value, const std::string& help = "");
        void SetDefaultString(const std::string& name, const std::string& value, const std::string& help = "");

        int GetInt(const std::string& name);
        int GetIntOr(const std::string& name, int value);
        void SetInt(const std::string& name, int value);
        float GetFloat(const std::string& name);
        float GetFloatOr(const std::string& name, float value);
        void SetFloat(const std::string& name, float value);
        bool GetBool(const std::string& name);
        bool GetBoolOr(const std::string& name, bool value);
        void SetBool(const std::string& name, bool value);
        std::string GetString(const std::string& name);
        std::string GetStringOr(const std::string& name, const std::string& value);
        void SetString(const std::string& name, const std::string& value);

    private:
        Core* m_core;
        std::string m_name;
        m64p_handle m_handle;

        ConfigSection(Core& core, const std::string& name, m64p_handle handle);
    };

    ConfigSection ConfigOpenSection(const std::string& name);

    std::filesystem::path GetSharedDataFilePath(const std::string& file);
    std::filesystem::path GetUserConfigPath();
    std::filesystem::path GetUserDataPath();
    std::filesystem::path GetUserCachePath();

    using DebugInitCallback = void(*)();
    using DebugUpdateCallback = void(*)(unsigned pc);
    using DebugVICallback = void(*)();

    void DebugSetCallbacks(DebugInitCallback init, DebugUpdateCallback update, DebugVICallback vi);
    m64p_dbg_runstate DebugGetRunState();
    void DebugSetRunState(m64p_dbg_runstate state);
    void DebugStep();
    std::string DebugDecodeOp(u32 inst, int pc);
    u64 DebugMemRead64(u32 addr);
    u32 DebugMemRead32(u32 addr);
    u16 DebugMemRead16(u32 addr);
    u8 DebugMemRead8(u32 addr);
    void DebugMemWrite64(u32 addr, u64 val);
    void DebugMemWrite32(u32 addr, u32 val);
    void DebugMemWrite16(u32 addr, u16 val);
    void DebugMemWrite8(u32 addr, u8 val);
    int DebugBpCount();
    int DebugBpLookup(u32 addr, u32 size, u32 flags);
    int DebugBpAddAddr(u32 addr);
    int DebugBpAddStruct(const m64p_breakpoint& bp);
    void DebugBpReplace(int index, const m64p_breakpoint& bp);
    void DebugBpRemoveAddr(u32 addr);
    void DebugBpRemoveIdx(int index);
    void DebugBpEnable(int index);
    void DebugBpDisable(int index);
    int DebugBpCheck(u32 addr);
    void DebugBpTriggeredBy(u32* flags, u32* accessed);
    int DebugGetPrevPC();
    int* DebugGetPCPtr();
    u32* DebugGetRegPtr();
    u32 DebugVirtualToPhysical(u32 addr);
    m64p_dbg_mem_type DebugGetMemType(u32 addr);
    m64p_dbg_mem_flags DebugGetMemFlags(u32 addr);

    void SetFrameCallback(ExtCallback frame);
    void SetVICallback(ExtCallback vi);
    void SetResetCallback(ExtIntCallback reset);
    void SetPauseLoopCallback(ExtCallback pause);
    void SetEventCallback(ExtIntCallback event);
    u8* GetDRAMPtr();
    std::size_t GetDRAMSize();
    u8* GetROMPtr();
    std::size_t GetROMSize();
    void InvalidateCachedCode();
    void InputKeyDown(int mod, int sym);
    void InputKeyUp(int mod, int sym);
    void GfxResizeOutput(int w, int h);
    void ResetInputPlugin();
    u32 ContGetInput(int index);
    void ContSetInput(int index, u32 input);
    u32 ContGetPluginFilter(int index);
    void ContSetPluginFilter(int index, u32 filter);

    u8 RDRAMRead8(u32 addr);
    u16 RDRAMRead16(u32 addr);
    u32 RDRAMRead32(u32 addr);
    u64 RDRAMRead64(u32 addr);
    u8* RDRAMReadBuffer(u32 addr, std::size_t len);
    void RDRAMWrite8(u32 addr, u8 val);
    void RDRAMWrite16(u32 addr, u16 val);
    void RDRAMWrite32(u32 addr, u32 val);
    void RDRAMWrite64(u32 addr, u64 val);
    void RDRAMWriteBuffer(u32 addr, u8* buf, std::size_t len);
    u8 ROMRead8(u32 addr);
    u16 ROMRead16(u32 addr);
    u32 ROMRead32(u32 addr);
    u8* ROMReadBuffer(u32 addr, std::size_t len);
    void ROMWrite8(u32 addr, u8 val);
    void ROMWrite16(u32 addr, u16 val);
    void ROMWrite32(u32 addr, u32 val);
    void ROMWriteBuffer(u32 addr, u8* buf, std::size_t len);

private:
    class Loader;

    std::unique_ptr<Loader> m_ld;
};

}
