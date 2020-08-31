#include "addon/param.h"
#include "addon/safe_call.h"
#include "frontend/app.h"

using namespace Param;

namespace Addon::M64P::Debugger {

inline Frontend::App& GetApp()
{
    return Frontend::App::GetInstance();
}

inline ::M64P::Core& GetCore()
{
    return GetApp().GetCore();
}

Napi::Value IsSupported(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), GetApp().IsDebuggerSupported());
}

Napi::Value IsEnabled(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info] {
        return FromBool(info.Env(), GetApp().IsDebuggerEnabled());
    });
}

Napi::Value IsInitialized(const Napi::CallbackInfo& info)
{
    return FromBool(info.Env(), GetApp().IsDebuggerInitialized());
}

Napi::Value GetRunState(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), GetCore().DebugGetRunState());
}

Napi::Value SetRunState(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info] {
        GetCore().DebugSetRunState(static_cast<m64p_dbg_runstate>(AsS32(info[0])));

        return info.Env().Undefined();
    });
}

Napi::Value Step(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info] {
        GetCore().DebugStep();

        return info.Env().Undefined();
    });
}

Napi::Value DecodeOp(const Napi::CallbackInfo& info)
{
    return FromStrUtf8(info.Env(), GetCore().DebugDecodeOp(AsU32(info[0]), AsS32(info[1])));
}

Napi::Value MemRead64(const Napi::CallbackInfo& info)
{
    return FromF64(info.Env(), static_cast<double>(GetCore().DebugMemRead64(AsU32(info[0]))));
}

Napi::Value MemRead32(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), GetCore().DebugMemRead32(AsU32(info[0])));
}

Napi::Value MemRead16(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), GetCore().DebugMemRead16(AsU32(info[0])));
}

Napi::Value MemRead8(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), GetCore().DebugMemRead8(AsU32(info[0])));
}

Napi::Value MemWrite64(const Napi::CallbackInfo& info)
{
    GetCore().DebugMemWrite64(AsU32(info[0]), static_cast<u64>(AsS64(info[1])));

    return info.Env().Undefined();
}

Napi::Value MemWrite32(const Napi::CallbackInfo& info)
{
    GetCore().DebugMemWrite32(AsU32(info[0]), AsU32(info[1]));

    return info.Env().Undefined();
}

Napi::Value MemWrite16(const Napi::CallbackInfo& info)
{
    GetCore().DebugMemWrite16(AsU32(info[0]), AsU32(info[1]));

    return info.Env().Undefined();
}

Napi::Value MemWrite8(const Napi::CallbackInfo& info)
{
    GetCore().DebugMemWrite8(AsU32(info[0]), AsU32(info[1]));

    return info.Env().Undefined();
}

Napi::Value BpCount(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), GetCore().DebugBpCount());
}

Napi::Value BpLookup(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), GetCore().DebugBpLookup(AsU32(info[0]), AsU32(info[1]), AsU32(info[2])));
}

Napi::Value BpAddAddress(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), GetCore().DebugBpAddAddr(AsU32(info[0])));
}

Napi::Value BpAddStruct(const Napi::CallbackInfo& info)
{
    auto obj = AsObject(info[0]);
    m64p_breakpoint bp;
    bp.address = AsU32(obj.Get("address"));
    bp.endaddr = AsU32(obj.Get("endAddress"));
    bp.flags = AsU32(obj.Get("flags"));

    return FromS32(info.Env(), GetCore().DebugBpAddStruct(bp));
}

Napi::Value BpReplace(const Napi::CallbackInfo& info)
{
    auto obj = AsObject(info[1]);
    m64p_breakpoint bp;
    bp.address = AsU32(obj.Get("address"));
    bp.endaddr = AsU32(obj.Get("endAddress"));
    bp.flags = AsU32(obj.Get("flags"));

    GetCore().DebugBpReplace(AsS32(info[0]), bp);

    return info.Env().Undefined();
}

Napi::Value BpRemoveAddress(const Napi::CallbackInfo& info)
{
    GetCore().DebugBpRemoveAddr(AsU32(info[0]));

    return info.Env().Undefined();
}

Napi::Value BpRemoveIndex(const Napi::CallbackInfo& info)
{
    GetCore().DebugBpRemoveIdx(AsS32(info[0]));

    return info.Env().Undefined();
}

Napi::Value BpEnable(const Napi::CallbackInfo& info)
{
    GetCore().DebugBpEnable(AsS32(info[0]));

    return info.Env().Undefined();
}

Napi::Value BpDisable(const Napi::CallbackInfo& info)
{
    GetCore().DebugBpDisable(AsS32(info[0]));

    return info.Env().Undefined();
}

Napi::Value BpCheck(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), GetCore().DebugBpCheck(AsU32(info[0])));
}

Napi::Value BpTriggeredBy(const Napi::CallbackInfo& info)
{
    u32 flags, accessed;
    GetCore().DebugBpTriggeredBy(&flags, &accessed);

    auto obj = Napi::Object::New(info.Env());
    obj.Set("flags", FromU32(info.Env(), flags));
    obj.Set("address", FromU32(info.Env(), accessed));

    return obj;
}

Napi::Value GetPrevPC(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), GetCore().DebugGetPrevPC());
}

Napi::Value ReadPC(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), *GetCore().DebugGetPCPtr());
}

Napi::Value WritePC(const Napi::CallbackInfo& info)
{
    *GetCore().DebugGetPCPtr() = AsS32(info[0]);

    return info.Env().Undefined();
}

Napi::Value ReadRegister(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), GetCore().DebugGetRegPtr()[AsS32(info[0])]);
}

Napi::Value WriteRegister(const Napi::CallbackInfo& info)
{
    GetCore().DebugGetRegPtr()[AsS32(info[0])] = AsU32(info[1]);

    return info.Env().Undefined();
}

Napi::Value VirtualToPhysical(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), GetCore().DebugVirtualToPhysical(AsU32(info[0])));
}

Napi::Value GetMemType(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), GetCore().DebugGetMemType(AsU32(info[0])));
}

Napi::Value GetMemFlags(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), GetCore().DebugGetMemFlags(AsU32(info[0])));
}

Napi::Object BuildExports(Napi::Env env, Napi::Object exports)
{
    exports.Set("isSupported", Napi::Function::New(env, IsSupported));
    exports.Set("isEnabled", Napi::Function::New(env, IsEnabled));
    exports.Set("isInitialized", Napi::Function::New(env, IsInitialized));

    exports.Set("getRunState", Napi::Function::New(env, GetRunState));
    exports.Set("setRunState", Napi::Function::New(env, SetRunState));
    exports.Set("step", Napi::Function::New(env, Step));
    exports.Set("decodeOp", Napi::Function::New(env, DecodeOp));
    exports.Set("memRead64", Napi::Function::New(env, MemRead64));
    exports.Set("memRead32", Napi::Function::New(env, MemRead32));
    exports.Set("memRead16", Napi::Function::New(env, MemRead16));
    exports.Set("memRead8", Napi::Function::New(env, MemRead8));
    exports.Set("memWrite64", Napi::Function::New(env, MemWrite64));
    exports.Set("memWrite32", Napi::Function::New(env, MemWrite32));
    exports.Set("memWrite16", Napi::Function::New(env, MemWrite16));
    exports.Set("memWrite8", Napi::Function::New(env, MemWrite8));
    exports.Set("bpCount", Napi::Function::New(env, BpCount));
    exports.Set("bpLookup", Napi::Function::New(env, BpLookup));
    exports.Set("bpAddAddress", Napi::Function::New(env, BpAddAddress));
    exports.Set("bpAddStruct", Napi::Function::New(env, BpAddStruct));
    exports.Set("bpReplace", Napi::Function::New(env, BpReplace));
    exports.Set("bpRemoveAddress", Napi::Function::New(env, BpRemoveAddress));
    exports.Set("bpRemoveIndex", Napi::Function::New(env, BpRemoveIndex));
    exports.Set("bpEnable", Napi::Function::New(env, BpEnable));
    exports.Set("bpDisable", Napi::Function::New(env, BpDisable));
    exports.Set("bpCheck", Napi::Function::New(env, BpCheck));
    exports.Set("bpTriggeredBy", Napi::Function::New(env, BpTriggeredBy));
    exports.Set("getPrevPC", Napi::Function::New(env, GetPrevPC));
    exports.Set("readPC", Napi::Function::New(env, ReadPC));
    exports.Set("writePC", Napi::Function::New(env, WritePC));
    exports.Set("readRegister", Napi::Function::New(env, ReadRegister));
    exports.Set("writeRegister", Napi::Function::New(env, WriteRegister));
    exports.Set("virtualToPhysical", Napi::Function::New(env, VirtualToPhysical));
    exports.Set("getMemType", Napi::Function::New(env, GetMemType));
    exports.Set("getMemFlags", Napi::Function::New(env, GetMemFlags));

    return exports;
}

}
