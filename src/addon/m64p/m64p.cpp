#include "addon/m64p/config.h"
#include "addon/m64p/debugger.h"
#include "addon/m64p/input.h"
#include "addon/m64p/memory.h"
#include "addon/param.h"
#include "addon/safe_call.h"
#include "common/file_util.h"
#include "frontend/app.h"

#include <fmt/format.h>
#include <SDL2/SDL_endian.h>

using namespace Param;

namespace Addon::M64P {

inline Frontend::App& GetApp()
{
    return Frontend::App::GetInstance();
}

inline ::M64P::Core& GetCore()
{
    return GetApp().GetCore();
}

Napi::Value AddCheat(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
		auto codes = AsArray(info[1]);

		if (!codes.IsEmpty()) {
			std::vector<m64p_cheat_code> codes_v(codes.Length());

			for (std::size_t i{}; i < codes_v.size(); ++i) {
				auto code = AsObject(codes.Get(static_cast<u32>(i)));

				codes_v[i].address = AsU32(code.Get("address"));
				codes_v[i].value = AsU32(code.Get("value"));
			}

			GetCore().AddCheat(AsStrUtf8(info[0]), codes_v.data(), codes_v.size());
		}
		else {
			GetCore().AddCheat(AsStrUtf8(info[0]), nullptr, 0);
        }

        return info.Env().Undefined();
    });
}

Napi::Value SetCheatEnabled(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
		GetCore().SetCheatEnabled(AsStrUtf8(info[0]), AsBool(info[1]));

        return info.Env().Undefined();
    });
}

Napi::Value GetROMSettingsFromCRCs(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
		m64p_rom_settings settings{};
		GetCore().GetROMSettings(&settings, AsS32(info[0]), AsS32(info[1]));

		auto ret = Napi::Object::New(info.Env());
		ret.Set("goodname", FromStrUtf8(info.Env(), reinterpret_cast<const char*>(settings.goodname)));
		ret.Set("md5", FromStrUtf8(info.Env(), reinterpret_cast<const char*>(settings.MD5)));
		ret.Set("savetype", FromU32(info.Env(), settings.savetype));
		ret.Set("status", FromU32(info.Env(), settings.status));
		ret.Set("players", FromU32(info.Env(), settings.players));
		ret.Set("rumble", FromU32(info.Env(), settings.rumble));
		ret.Set("transferpak", FromU32(info.Env(), settings.transferpak));
		ret.Set("mempak", FromU32(info.Env(), settings.mempak));
		ret.Set("biopak", FromU32(info.Env(), settings.biopak));

		return ret;
    });
}

Napi::Value OpenROMFromFile(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        try {
            auto v = FileUtil::ReadAllBytes(AsPath(info[0]));

            if (!info[1].IsUndefined())
                v.resize(AsU32(info[1]));

            GetCore().OpenROM(v.data(), v.size());
        }
        catch (const std::exception& e) {
            throw std::runtime_error{fmt::format("Failed to open ROM from file. {}", e.what())};
        }

        return info.Env().Undefined();
    });
}

Napi::Value OpenROMFromMemory(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        auto buf = info[0].As<Napi::Uint8Array>();
        std::vector<u8> v(buf.ByteLength());
        std::copy(buf.Data(), buf.Data() + buf.ByteLength(), v.begin());

        if (!info[1].IsUndefined())
            v.resize(AsU32(info[1]));

        GetCore().OpenROM(v.data(), v.size());

        return info.Env().Undefined();
    });
}

Napi::Value CloseROM(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().CloseROM();

        return info.Env().Undefined();
    });
}

Napi::Value GetROMHeader(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        m64p_rom_header header{};
	    GetCore().GetROMHeader(&header);

		auto ret = Napi::Object::New(info.Env());
		ret.Set("init_PI_BSB_DOM1_LAT_REG", FromU32(info.Env(), header.init_PI_BSB_DOM1_LAT_REG));
		ret.Set("init_PI_BSB_DOM1_PGS_REG", FromU32(info.Env(), header.init_PI_BSB_DOM1_PGS_REG));
		ret.Set("init_PI_BSB_DOM1_PWD_REG", FromU32(info.Env(), header.init_PI_BSB_DOM1_PWD_REG));
		ret.Set("init_PI_BSB_DOM1_PGS_REG2", FromU32(info.Env(), header.init_PI_BSB_DOM1_PGS_REG2));
		ret.Set("clockRate", FromU32(info.Env(), SDL_Swap32(header.ClockRate)));
		ret.Set("pc", FromU32(info.Env(), SDL_Swap32(header.PC)));
		ret.Set("release", FromU32(info.Env(), SDL_Swap32(header.Release)));
		ret.Set("crc1", FromU32(info.Env(), SDL_Swap32(header.CRC1)));
		ret.Set("crc2", FromU32(info.Env(), SDL_Swap32(header.CRC2)));
		ret.Set("name", FromStrUtf8(info.Env(), reinterpret_cast<const char*>(header.Name)));
		ret.Set("manufacturerId", FromU32(info.Env(), SDL_Swap32(header.Manufacturer_ID)));
		ret.Set("cartridgeId", FromU32(info.Env(), SDL_Swap16(header.Cartridge_ID)));
		ret.Set("countryCode", FromU32(info.Env(), SDL_Swap16(header.Country_code)));

		return ret;
    });
}

Napi::Value GetROMSettings(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        m64p_rom_settings settings{};
		GetCore().GetROMSettings(&settings);

		auto ret = Napi::Object::New(info.Env());
		ret.Set("goodname", FromStrUtf8(info.Env(), reinterpret_cast<const char*>(settings.goodname)));
		ret.Set("md5", FromStrUtf8(info.Env(), reinterpret_cast<const char*>(settings.MD5)));
		ret.Set("savetype", FromU32(info.Env(), settings.savetype));
		ret.Set("status", FromU32(info.Env(), settings.status));
		ret.Set("players", FromU32(info.Env(), settings.players));
		ret.Set("rumble", FromU32(info.Env(), settings.rumble));
		ret.Set("transferpak", FromU32(info.Env(), settings.transferpak));
		ret.Set("mempak", FromU32(info.Env(), settings.mempak));
		ret.Set("biopak", FromU32(info.Env(), settings.biopak));

		return ret;
    });
}

Napi::Value Pause(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().Pause();

        return info.Env().Undefined();
    });
}

Napi::Value Resume(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().Resume();

        return info.Env().Undefined();
    });
}

Napi::Value SetStateSlot(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().SetStateSlot(AsS32(info[0]));

        return info.Env().Undefined();
    });
}

Napi::Value LoadState(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().LoadState();

        return info.Env().Undefined();
    });
}

Napi::Value LoadStateFromSlot(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().LoadState(AsS32(info[0]));

        return info.Env().Undefined();
    });
}

Napi::Value LoadStateFromFile(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().LoadState(AsPath(info[0]));

        return info.Env().Undefined();
    });
}

Napi::Value SaveState(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().SaveState();

        return info.Env().Undefined();
    });
}

Napi::Value SaveStateToSlot(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().SaveState(AsS32(info[0]));

        return info.Env().Undefined();
    });
}

Napi::Value SaveStateToFile(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().SaveState(AsPath(info[0]));

        return info.Env().Undefined();
    });
}

Napi::Value TakeNextScreenshot(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().TakeNextScreenshot();

        return info.Env().Undefined();
    });
}

Napi::Value SoftReset(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().Reset(false);

        return info.Env().Undefined();
    });
}

Napi::Value HardReset(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().Reset(true);

        return info.Env().Undefined();
    });
}

Napi::Value AdvanceFrame(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().AdvanceFrame();

        return info.Env().Undefined();
    });
}

Napi::Value GetEmuState(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        return FromS32(info.Env(), GetCore().GetEmuState());
    });
}

Napi::Value GetVideoMode(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        return FromS32(info.Env(), GetCore().GetVideoMode());
    });
}

Napi::Value GetStateSlot(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        return FromS32(info.Env(), GetCore().GetStateSlot());
    });
}

Napi::Value GetSpeedFactor(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        return FromS32(info.Env(), GetCore().GetSpeedFactor());
    });
}

Napi::Value SetSpeedFactor(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().SetSpeedFactor(AsS32(info[0]));

        return info.Env().Undefined();
    });
}

Napi::Value IsSpeedLimiterEnabled(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        return FromBool(info.Env(), GetCore().IsSpeedLimiterEnabled());
    });
}

Napi::Value SetSpeedLimiterEnabled(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().SetSpeedLimiterEnabled(AsBool(info[0]));

        return info.Env().Undefined();
    });
}

Napi::Value GetVideoSize(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        const auto size = GetCore().GetVideoSize();

		auto ret = Napi::Object::New(info.Env());
		ret.Set("width", FromU32(info.Env(), size.uiWidth));
		ret.Set("height", FromU32(info.Env(), size.uiHeight));

		return ret;
    });
}

Napi::Value GetAudioVolume(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        return FromS32(info.Env(), GetCore().GetAudioVolume());
    });
}

Napi::Value SetAudioVolume(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().SetAudioVolume(AsS32(info[0]));

        return info.Env().Undefined();
    });
}

Napi::Value IsAudioMuted(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        return FromBool(info.Env(), GetCore().IsAudioMuted());
    });
}

Napi::Value SetAudioMuted(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().SetAudioMuted(AsBool(info[0]));

        return info.Env().Undefined();
    });
}

Napi::Value IsGSButtonPressed(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        return FromBool(info.Env(), GetCore().IsGSButtonPressed());
    });
}

Napi::Value SetGSButtonPressed(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetCore().SetGSButtonPressed(AsBool(info[0]));

        return info.Env().Undefined();
    });
}

Napi::Value HasInputFocus(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), GetApp().HasEmuInputFocus());
}

Napi::Value GetNumElapsedFrames(const Napi::CallbackInfo& info)
{
    return FromF64(info.Env(), static_cast<f64>(GetApp().GetElapsedFrameCount()));
}

Napi::Object BuildExports(Napi::Env env, Napi::Object exports)
{
    exports.Set("Config", Config::BuildExports(env, Napi::Object::New(env)));
    exports.Set("Debugger", Debugger::BuildExports(env, Napi::Object::New(env)));
    exports.Set("Input", Input::BuildExports(env, Napi::Object::New(env)));
    exports.Set("Memory", Memory::BuildExports(env, Napi::Object::New(env)));

    exports.Set("addCheat", Napi::Function::New(env, AddCheat));
    exports.Set("setCheatEnabled", Napi::Function::New(env, SetCheatEnabled));
    exports.Set("getRomSettingsFromCRCs", Napi::Function::New(env, GetROMSettingsFromCRCs));
    exports.Set("openRomFromFile", Napi::Function::New(env, OpenROMFromFile));
    exports.Set("openRomFromMemory", Napi::Function::New(env, OpenROMFromMemory));
    exports.Set("closeRom", Napi::Function::New(env, CloseROM));
    exports.Set("getRomHeader", Napi::Function::New(env, GetROMHeader));
    exports.Set("getRomSettings", Napi::Function::New(env, GetROMSettings));
    exports.Set("pause", Napi::Function::New(env, Pause));
    exports.Set("resume", Napi::Function::New(env, Resume));
    exports.Set("setStateSlot", Napi::Function::New(env, SetStateSlot));
    exports.Set("loadState", Napi::Function::New(env, LoadState));
    exports.Set("loadStateFromSlot", Napi::Function::New(env, LoadStateFromSlot));
    exports.Set("loadStateFromFile", Napi::Function::New(env, LoadStateFromFile));
    exports.Set("saveState", Napi::Function::New(env, SaveState));
    exports.Set("saveStateToSlot", Napi::Function::New(env, SaveStateToSlot));
    exports.Set("saveStateToFile", Napi::Function::New(env, SaveStateToFile));
    exports.Set("takeNextScreenshot", Napi::Function::New(env, TakeNextScreenshot));
    exports.Set("softReset", Napi::Function::New(env, SoftReset));
    exports.Set("hardReset", Napi::Function::New(env, HardReset));
    exports.Set("advanceFrame", Napi::Function::New(env, AdvanceFrame));
    exports.Set("getEmuState", Napi::Function::New(env, GetEmuState));
    exports.Set("getVideoMode", Napi::Function::New(env, GetVideoMode));
    exports.Set("getStateSlot", Napi::Function::New(env, GetStateSlot));
    exports.Set("getSpeedFactor", Napi::Function::New(env, GetSpeedFactor));
    exports.Set("setSpeedFactor", Napi::Function::New(env, SetSpeedFactor));
    exports.Set("isSpeedLimiterEnabled", Napi::Function::New(env, IsSpeedLimiterEnabled));
    exports.Set("setSpeedLimiterEnabled", Napi::Function::New(env, SetSpeedLimiterEnabled));
    exports.Set("getVideoSize", Napi::Function::New(env, GetVideoSize));
    exports.Set("getAudioVolume", Napi::Function::New(env, GetAudioVolume));
    exports.Set("setAudioVolume", Napi::Function::New(env, SetAudioVolume));
    exports.Set("isAudioMuted", Napi::Function::New(env, IsAudioMuted));
    exports.Set("setAudioMuted", Napi::Function::New(env, SetAudioMuted));
    exports.Set("isGSButtonPressed", Napi::Function::New(env, IsGSButtonPressed));
    exports.Set("setGSButtonPressed", Napi::Function::New(env, SetGSButtonPressed));

    exports.Set("hasInputFocus", Napi::Function::New(env, HasInputFocus));
    exports.Set("getNumElapsedFrames", Napi::Function::New(env, GetNumElapsedFrames));

    return exports;
}

}
