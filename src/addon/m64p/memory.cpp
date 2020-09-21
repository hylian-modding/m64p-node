#include "addon/param.h"
#include "frontend/app.h"

#include <bitset>

using namespace Param;

namespace Addon::M64P::Memory {

inline ::M64P::Core& GetCore()
{
    return Frontend::App::GetInstance().GetCore();
}

inline u32 PackF32(f32 v)
{
    u32 m;
    std::memcpy(&m, &v, 4);
    return m;
}

inline f32 UnpackF32(u32 v)
{
    f32 m;
    std::memcpy(&m, &v, 4);
    return m;
}

Napi::Value GetRDRAMBuffer(const Napi::CallbackInfo& info)
{
    return Napi::ArrayBuffer::New(info.Env(),
        const_cast<std::uint8_t*>(GetCore().GetDRAMPtr()), GetCore().GetDRAMSize());
}

Napi::Value GetROMBuffer(const Napi::CallbackInfo& info)
{
    return Napi::ArrayBuffer::New(info.Env(),
        const_cast<std::uint8_t*>(GetCore().GetROMPtr()), GetCore().GetROMSize());
}

Napi::Value InvalidateCachedCode(const Napi::CallbackInfo& info)
{
    GetCore().InvalidateCachedCode();

    return info.Env().Undefined();
}

Napi::Value RDRAMRead8(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), GetCore().RDRAMRead8(AsU32(info[0])));
}

Napi::Value RDRAMRead16(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), GetCore().RDRAMRead16(AsU32(info[0])));
}

Napi::Value RDRAMRead32(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), GetCore().RDRAMRead32(AsU32(info[0])));
}

Napi::Value RDRAMReadS8(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), static_cast<s8>(GetCore().RDRAMRead8(AsU32(info[0]))));
}

Napi::Value RDRAMReadS16(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), static_cast<s16>(GetCore().RDRAMRead16(AsU32(info[0]))));
}

Napi::Value RDRAMReadS32(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), static_cast<s32>(GetCore().RDRAMRead32(AsU32(info[0]))));
}

Napi::Value RDRAMReadF32(const Napi::CallbackInfo& info)
{
    auto v = GetCore().RDRAMRead32(AsU32(info[0]));

    return FromF32(info.Env(), UnpackF32(v));
}

Napi::Value RDRAMReadBuffer(const Napi::CallbackInfo& info)
{
    auto len = AsU32(info[1]);
    auto buf = GetCore().RDRAMReadBuffer(AsU32(info[0]), len);
    auto ret = Napi::Buffer<u8>::Copy(info.Env(), buf, len);
    std::free(buf);

    return ret;
}

Napi::Value RDRAMWrite8(const Napi::CallbackInfo& info)
{
    GetCore().RDRAMWrite8(AsU32(info[0]), AsU32(info[1]));

    return info.Env().Undefined();
}

Napi::Value RDRAMWrite16(const Napi::CallbackInfo& info)
{
    GetCore().RDRAMWrite16(AsU32(info[0]), AsU32(info[1]));

    return info.Env().Undefined();
}

Napi::Value RDRAMWrite32(const Napi::CallbackInfo& info)
{
    GetCore().RDRAMWrite32(AsU32(info[0]), AsU32(info[1]));

    return info.Env().Undefined();
}

Napi::Value RDRAMWriteF32(const Napi::CallbackInfo& info)
{
    auto v = AsF32(info[1]);
    GetCore().RDRAMWrite32(AsU32(info[0]), PackF32(v));

    return info.Env().Undefined();
}

Napi::Value RDRAMWriteBuffer(const Napi::CallbackInfo& info)
{
    auto buf = info[1].As<Napi::Uint8Array>();
    GetCore().RDRAMWriteBuffer(AsU32(info[0]), buf.Data(), buf.ByteLength());

    return info.Env().Undefined();
}

Napi::Value ROMRead8(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), GetCore().ROMRead8(AsU32(info[0])));
}

Napi::Value ROMRead16(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), GetCore().ROMRead16(AsU32(info[0])));
}

Napi::Value ROMRead32(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), GetCore().ROMRead32(AsU32(info[0])));
}

Napi::Value ROMReadS8(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), static_cast<s8>(GetCore().ROMRead8(AsU32(info[0]))));
}

Napi::Value ROMReadS16(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), static_cast<s16>(GetCore().ROMRead16(AsU32(info[0]))));
}

Napi::Value ROMReadS32(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), static_cast<s32>(GetCore().ROMRead32(AsU32(info[0]))));
}

Napi::Value ROMReadBuffer(const Napi::CallbackInfo& info)
{
    auto len = AsU32(info[1]);
    auto buf = GetCore().ROMReadBuffer(AsU32(info[0]), len);
    auto ret = Napi::Buffer<u8>::Copy(info.Env(), buf, len);
    std::free(buf);

    return ret;
}

Napi::Value ROMWrite8(const Napi::CallbackInfo& info)
{
    GetCore().ROMWrite8(AsU32(info[0]), AsU32(info[1]));

    return info.Env().Undefined();
}

Napi::Value ROMWrite16(const Napi::CallbackInfo& info)
{
    GetCore().ROMWrite16(AsU32(info[0]), AsU32(info[1]));

    return info.Env().Undefined();
}

Napi::Value ROMWrite32(const Napi::CallbackInfo& info)
{
    GetCore().ROMWrite32(AsU32(info[0]), AsU32(info[1]));

    return info.Env().Undefined();
}

Napi::Value ROMWriteBuffer(const Napi::CallbackInfo& info)
{
    auto buf = info[1].As<Napi::Uint8Array>();
    GetCore().ROMWriteBuffer(AsU32(info[0]), buf.Data(), buf.ByteLength());

    return info.Env().Undefined();
}

inline Napi::Value RDRAMReadBit8_(Napi::Env env, u32 addr, u32 offset)
{
    return FromBool(env, std::bitset<8>{GetCore().RDRAMRead8(addr)}.test(7 - offset));
}

Napi::Value RDRAMReadBits8_(Napi::Env env, u32 addr)
{
    auto buf = Napi::Buffer<u8>::New(env, 8);
    std::bitset<8> v = GetCore().RDRAMRead8(addr);

	for (u32 j{}; j < 8; ++j)
		buf.Set(j, v.test(7 - j));

    return buf;
}

Napi::Value RDRAMReadBitsBuffer_(Napi::Env env, u32 addr, u32 len)
{
    auto buf = Napi::Buffer<u8>::New(env, len * 8);

    for (u32 i{}; i < len; ++i) {
		std::bitset<8> v = GetCore().RDRAMRead8(addr + i);

		for (u32 j{}; j < 8; ++j)
			buf.Set(i * 8 + j, v.test(7 - j));
	}

    return buf;
}

void RDRAMWriteBit8_(u32 addr, u32 offset, bool set)
{
    std::bitset<8> v = GetCore().RDRAMRead8(addr);
	v.set(7 - offset, set);
	GetCore().RDRAMWrite8(addr, static_cast<u8>(v.to_ulong()));
}

void RDRAMWriteBits8_(u32 addr, const Napi::Uint8Array& buf)
{
	std::bitset<8> v;

	for (u32 j{}; j < 8; ++j)
		v.set(7 - j, ToBool(buf.Get(j)));

	GetCore().RDRAMWrite8(addr, static_cast<u8>(v.to_ulong()));
}

void RDRAMWriteBitsBuffer_(u32 addr, const Napi::Uint8Array& buf)
{
    for (u32 i{}; i < buf.ElementLength() / 8; ++i) {
		std::bitset<8> v;

		for (u32 j{}; j < 8; ++j)
			v.set(7 - j, ToBool(buf.Get(i * 8 + j)));

		GetCore().RDRAMWrite8(addr + i, static_cast<u8>(v.to_ulong()));
	}
}

Napi::Value RDRAMReadBit8(const Napi::CallbackInfo& info)
{
    return RDRAMReadBit8_(info.Env(), AsU32(info[0]), AsU32(info[1]));
}

Napi::Value RDRAMReadBits8(const Napi::CallbackInfo& info)
{
    return RDRAMReadBits8_(info.Env(), AsU32(info[0]));
}

Napi::Value RDRAMReadBitsBuffer(const Napi::CallbackInfo& info)
{
    return RDRAMReadBitsBuffer_(info.Env(), AsU32(info[0]), AsU32(info[1]));
}

Napi::Value RDRAMWriteBit8(const Napi::CallbackInfo& info)
{
    RDRAMWriteBit8_(AsU32(info[0]), AsU32(info[1]), AsBool(info[2]));

    return info.Env().Undefined();
}

Napi::Value RDRAMWriteBits8(const Napi::CallbackInfo& info)
{
    RDRAMWriteBits8_(AsU32(info[0]), info[1].As<Napi::Uint8Array>());

    return info.Env().Undefined();
}

Napi::Value RDRAMWriteBitsBuffer(const Napi::CallbackInfo& info)
{
    RDRAMWriteBitsBuffer_(AsU32(info[0]), info[1].As<Napi::Uint8Array>());

    return info.Env().Undefined();
}

Napi::Value DerefPtr(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), GetCore().RDRAMRead32(AsU32(info[0])) & 0x00ffffff);
}

inline u32 DerefPtr_(const Napi::CallbackInfo& info)
{
    return (GetCore().RDRAMRead32(AsU32(info[0])) & 0x00ffffff) + AsU32(info[1]);
}

Napi::Value RDRAMReadPtr8(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), GetCore().RDRAMRead8(DerefPtr_(info)));
}

Napi::Value RDRAMReadPtr16(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), GetCore().RDRAMRead16(DerefPtr_(info)));
}

Napi::Value RDRAMReadPtr32(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), GetCore().RDRAMRead32(DerefPtr_(info)));
}

Napi::Value RDRAMReadPtrS8(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), static_cast<s8>(GetCore().RDRAMRead8(DerefPtr_(info))));
}

Napi::Value RDRAMReadPtrS16(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), static_cast<s16>(GetCore().RDRAMRead16(DerefPtr_(info))));
}

Napi::Value RDRAMReadPtrS32(const Napi::CallbackInfo& info)
{
    return FromS32(info.Env(), static_cast<s32>(GetCore().RDRAMRead32(DerefPtr_(info))));
}

Napi::Value RDRAMReadPtrF32(const Napi::CallbackInfo& info)
{
    auto v = GetCore().RDRAMRead32(DerefPtr_(info));

    return FromF32(info.Env(), UnpackF32(v));
}

Napi::Value RDRAMReadPtrBuffer(const Napi::CallbackInfo& info)
{
    auto len = AsU32(info[2]);
    auto buf = GetCore().RDRAMReadBuffer(DerefPtr_(info), len);
    auto ret = Napi::Buffer<u8>::Copy(info.Env(), buf, len);
    std::free(buf);

    return ret;
}

Napi::Value RDRAMWritePtr8(const Napi::CallbackInfo& info)
{
    GetCore().RDRAMWrite8(DerefPtr_(info), AsU32(info[2]));

    return info.Env().Undefined();
}

Napi::Value RDRAMWritePtr16(const Napi::CallbackInfo& info)
{
    GetCore().RDRAMWrite16(DerefPtr_(info), AsU32(info[2]));

    return info.Env().Undefined();
}

Napi::Value RDRAMWritePtr32(const Napi::CallbackInfo& info)
{
    GetCore().RDRAMWrite32(DerefPtr_(info), AsU32(info[2]));

    return info.Env().Undefined();
}

Napi::Value RDRAMWritePtrF32(const Napi::CallbackInfo& info)
{
    auto v = AsF32(info[2]);
    GetCore().RDRAMWrite32(DerefPtr_(info), PackF32(v));

    return info.Env().Undefined();
}

Napi::Value RDRAMWritePtrBuffer(const Napi::CallbackInfo& info)
{
    auto buf = info[2].As<Napi::Uint8Array>();
    GetCore().RDRAMWriteBuffer(DerefPtr_(info), buf.Data(), buf.ByteLength());

    return info.Env().Undefined();
}

Napi::Value RDRAMReadPtrBit8(const Napi::CallbackInfo& info)
{
    return RDRAMReadBit8_(info.Env(), DerefPtr_(info), AsU32(info[2]));
}

Napi::Value RDRAMReadPtrBits8(const Napi::CallbackInfo& info)
{
    return RDRAMReadBits8_(info.Env(), DerefPtr_(info));
}

Napi::Value RDRAMReadPtrBitsBuffer(const Napi::CallbackInfo& info)
{
    return RDRAMReadBitsBuffer_(info.Env(), DerefPtr_(info), AsU32(info[2]));
}

Napi::Value RDRAMWritePtrBit8(const Napi::CallbackInfo& info)
{
    RDRAMWriteBit8_(DerefPtr_(info), AsU32(info[2]), AsBool(info[3]));

    return info.Env().Undefined();
}

Napi::Value RDRAMWritePtrBits8(const Napi::CallbackInfo& info)
{
    RDRAMWriteBits8_(DerefPtr_(info), info[2].As<Napi::Uint8Array>());

    return info.Env().Undefined();
}

Napi::Value RDRAMWritePtrBitsBuffer(const Napi::CallbackInfo& info)
{
    RDRAMWriteBitsBuffer_(DerefPtr_(info), info[2].As<Napi::Uint8Array>());

    return info.Env().Undefined();
}

Napi::Value BitCount8(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), static_cast<u32>(std::bitset<8>{AsU32(info[0])}.count()));
}

Napi::Value BitCount16(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), static_cast<u32>(std::bitset<16>{AsU32(info[0])}.count()));
}

Napi::Value BitCount32(const Napi::CallbackInfo& info)
{
    return FromU32(info.Env(), static_cast<u32>(std::bitset<32>{AsU32(info[0])}.count()));
}

Napi::Value BitCountBuffer(const Napi::CallbackInfo& info)
{
    auto buf = info[0].As<Napi::Uint8Array>();
    auto off = AsU32(info[1]);
    auto len = AsU32(info[2]);
    u32 count{};

    if (len == 0)
        len = buf.ByteLength() - off;

    for (u32 i = off; i < off + len; ++i)
        count += static_cast<u32>(std::bitset<8>{AsU32(buf.Get(i))}.count());

    return FromU32(info.Env(), count);
}

Napi::Object BuildExports(Napi::Env env, Napi::Object exports)
{
    exports.Set("getRdramBuffer", Napi::Function::New(env, GetRDRAMBuffer));
    exports.Set("getRomBuffer", Napi::Function::New(env, GetROMBuffer));
    exports.Set("invalidateCachedCode", Napi::Function::New(env, InvalidateCachedCode));

    exports.Set("rdramRead8", Napi::Function::New(env, RDRAMRead8));
    exports.Set("rdramRead16", Napi::Function::New(env, RDRAMRead16));
    exports.Set("rdramRead32", Napi::Function::New(env, RDRAMRead32));
    exports.Set("rdramReadS8", Napi::Function::New(env, RDRAMReadS8));
    exports.Set("rdramReadS16", Napi::Function::New(env, RDRAMReadS16));
    exports.Set("rdramReadS32", Napi::Function::New(env, RDRAMReadS32));
    exports.Set("rdramReadF32", Napi::Function::New(env, RDRAMReadF32));
    exports.Set("rdramReadBuffer", Napi::Function::New(env, RDRAMReadBuffer));
    exports.Set("rdramWrite8", Napi::Function::New(env, RDRAMWrite8));
    exports.Set("rdramWrite16", Napi::Function::New(env, RDRAMWrite16));
    exports.Set("rdramWrite32", Napi::Function::New(env, RDRAMWrite32));
    exports.Set("rdramWriteF32", Napi::Function::New(env, RDRAMWriteF32));
    exports.Set("rdramWriteBuffer", Napi::Function::New(env, RDRAMWriteBuffer));
    exports.Set("romRead8", Napi::Function::New(env, ROMRead8));
    exports.Set("romRead16", Napi::Function::New(env, ROMRead16));
    exports.Set("romRead32", Napi::Function::New(env, ROMRead32));
    exports.Set("romReadS8", Napi::Function::New(env, ROMReadS8));
    exports.Set("romReadS16", Napi::Function::New(env, ROMReadS16));
    exports.Set("romReadS32", Napi::Function::New(env, ROMReadS32));
    exports.Set("romReadBuffer", Napi::Function::New(env, ROMReadBuffer));
    exports.Set("romWrite8", Napi::Function::New(env, ROMWrite8));
    exports.Set("romWrite16", Napi::Function::New(env, ROMWrite16));
    exports.Set("romWrite32", Napi::Function::New(env, ROMWrite32));
    exports.Set("romWriteBuffer", Napi::Function::New(env, ROMWriteBuffer));
    exports.Set("rdramReadBit8", Napi::Function::New(env, RDRAMReadBit8));
    exports.Set("rdramReadBits8", Napi::Function::New(env, RDRAMReadBits8));
    exports.Set("rdramReadBitsBuffer", Napi::Function::New(env, RDRAMReadBitsBuffer));
    exports.Set("rdramWriteBit8", Napi::Function::New(env, RDRAMWriteBit8));
    exports.Set("rdramWriteBits8", Napi::Function::New(env, RDRAMWriteBits8));
    exports.Set("rdramWriteBitsBuffer", Napi::Function::New(env, RDRAMWriteBitsBuffer));

    exports.Set("dereferencePointer", Napi::Function::New(env, DerefPtr));
    exports.Set("rdramReadPtr8", Napi::Function::New(env, RDRAMReadPtr8));
    exports.Set("rdramReadPtr16", Napi::Function::New(env, RDRAMReadPtr16));
    exports.Set("rdramReadPtr32", Napi::Function::New(env, RDRAMReadPtr32));
    exports.Set("rdramReadPtrS8", Napi::Function::New(env, RDRAMReadPtrS8));
    exports.Set("rdramReadPtrS16", Napi::Function::New(env, RDRAMReadPtrS16));
    exports.Set("rdramReadPtrS32", Napi::Function::New(env, RDRAMReadPtrS32));
    exports.Set("rdramReadPtrF32", Napi::Function::New(env, RDRAMReadPtrF32));
    exports.Set("rdramReadPtrBuffer", Napi::Function::New(env, RDRAMReadPtrBuffer));
    exports.Set("rdramWritePtr8", Napi::Function::New(env, RDRAMWritePtr8));
    exports.Set("rdramWritePtr16", Napi::Function::New(env, RDRAMWritePtr16));
    exports.Set("rdramWritePtr32", Napi::Function::New(env, RDRAMWritePtr32));
    exports.Set("rdramWritePtrF32", Napi::Function::New(env, RDRAMWritePtr32));
    exports.Set("rdramWritePtrBuffer", Napi::Function::New(env, RDRAMWritePtrBuffer));
    exports.Set("rdramReadPtrBit8", Napi::Function::New(env, RDRAMReadPtrBit8));
    exports.Set("rdramReadPtrBits8", Napi::Function::New(env, RDRAMReadPtrBits8));
    exports.Set("rdramReadPtrBitsBuffer", Napi::Function::New(env, RDRAMReadPtrBitsBuffer));
    exports.Set("rdramWritePtrBit8", Napi::Function::New(env, RDRAMWritePtrBit8));
    exports.Set("rdramWritePtrBits8", Napi::Function::New(env, RDRAMWritePtrBits8));
    exports.Set("rdramWritePtrBitsBuffer", Napi::Function::New(env, RDRAMWritePtrBitsBuffer));

    exports.Set("bitCount8", Napi::Function::New(env, BitCount8));
    exports.Set("bitCount16", Napi::Function::New(env, BitCount16));
    exports.Set("bitCount32", Napi::Function::New(env, BitCount32));
    exports.Set("bitCountBuffer", Napi::Function::New(env, BitCountBuffer));

    return exports;
}

}
