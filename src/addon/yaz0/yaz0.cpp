#include "common/types.h"

#include <napi.h>
#include <SDL2/SDL_endian.h>

namespace Addon::Yaz0 {

struct Ret {
    int src_pos;
    int dst_pos;
};

static void Dec(u8* src, u8* dst, int uncompressed_size)
{
    int src_place{}, dst_place{};
    u32 valid_bit_count{};
    u8 curr_code_byte{};

    while (dst_place < uncompressed_size) {
        if (valid_bit_count == 0) {
            curr_code_byte = src[src_place++];
            valid_bit_count = 8;
        }

        if ((curr_code_byte & 0x80) != 0) {
            dst[dst_place++] = src[src_place++];
        }
        else {
            u8 byte1 = src[src_place++];
            u8 byte2 = src[src_place++];

            u32 dist = ((byte1 & 0xf) << 8) | byte2;
            u32 copy_src = dst_place - (dist + 1);

            u32 num_bytes = byte1 >> 4;

            if (num_bytes == 0)
                num_bytes = src[src_place++] + 0x12;
            else
                num_bytes += 2;

            for (std::size_t i{}; i < num_bytes; ++i)
                dst[dst_place++] = dst[copy_src++];
        }

        curr_code_byte <<= 1;
        --valid_bit_count;
    }
}

static u32 SimpleEnc(u8* src, int size, int pos, u32* p_match_pos)
{
    int start_pos = pos - 0x1000;
    u32 num_bytes = 1, match_pos{};

    if (start_pos < 0)
        start_pos = 0;

    for (std::size_t i = start_pos; i < static_cast<std::size_t>(pos); ++i) {
        std::size_t j{};

        for (; j < static_cast<std::size_t>(size - pos); ++j) {
            if (src[i + j] != src[j + pos])
                break;
        }

        if (j > num_bytes) {
            num_bytes = static_cast<u32>(j);
            match_pos = static_cast<u32>(i);
        }
    }

    *p_match_pos = match_pos;

    if (num_bytes == 2)
        num_bytes = 1;

    return num_bytes;
}

static u32 NintendoEnc(u8* src, int size, int pos, u32* p_match_pos)
{
    u32 num_bytes = 1;
    static u32 num_bytes1, match_pos;
    static int prev_flag;

    if (prev_flag == 1) {
        *p_match_pos = match_pos;
        prev_flag = 0;
        return num_bytes1;
    }

    prev_flag = 0;
    num_bytes = SimpleEnc(src, size, pos, &match_pos);
    *p_match_pos = match_pos;

    if (num_bytes >= 3) {
        num_bytes1 = SimpleEnc(src, size, pos + 1, &match_pos);

        if (num_bytes1 >= num_bytes + 2) {
            num_bytes = 1;
            prev_flag = 1;
        }
    }

    return num_bytes;
}

static int Enc(u8* src, int src_size, u8* t_buf)
{
    Ret r{};
    u8 dst[24]{};
    std::size_t b_offset{};
    int dst_size{}, percent = -1;

    u32 valid_bit_count{};
    u8 curr_code_byte{};

    while (r.src_pos < src_size) {
        u32 match_pos{};
        u32 num_bytes = NintendoEnc(src, src_size, r.src_pos, &match_pos);

        if (num_bytes < 3) {
            dst[r.dst_pos++] = src[r.src_pos++];
            curr_code_byte |= (0x80 >> valid_bit_count);
        }
        else {
            u32 dist = r.src_pos - match_pos - 1;
            u8 byte1{}, byte2{}, byte3{};

            if (num_bytes >= 0x12) {
                byte1 = 0 | (dist >> 8);
                byte2 = dist & 0xff;
                dst[r.dst_pos++] = byte1;
                dst[r.dst_pos++] = byte2;

                if (num_bytes > 0xff + 0x12)
                    num_bytes = 0xff + 0x12;

                byte3 = num_bytes - 0x12;
                dst[r.dst_pos++] = byte3;
            }
            else {
                byte1 = ((num_bytes - 2) << 4) | (dist >> 8);
                byte2 = dist & 0xff;
                dst[r.dst_pos++] = byte1;
                dst[r.dst_pos++] = byte2;
            }

            r.src_pos += num_bytes;
        }

        ++valid_bit_count;

        if (valid_bit_count == 8) {
            std::memcpy(t_buf + b_offset, &curr_code_byte, 1);
            ++b_offset;

            std::memcpy(t_buf + b_offset, dst, r.dst_pos);
            b_offset += r.dst_pos;

            dst_size += r.dst_pos + 1;

            curr_code_byte = 0;
            valid_bit_count = 0;
            r.dst_pos = 0;
        }

        if ((r.src_pos + 1) * 100 / src_size != percent)
            percent = (r.src_pos + 1) * 100 / src_size;
    }

    if (valid_bit_count > 0) {
        std::memcpy(t_buf + b_offset, &curr_code_byte, 1);
        b_offset += 1;

        std::memcpy(t_buf + b_offset, dst, r.dst_pos);
        b_offset += r.dst_pos;

        dst_size += r.dst_pos + 1;

        curr_code_byte = 0;
        valid_bit_count = 0;
        r.dst_pos = 0;
    }

    return dst_size;
}

Napi::Value Encode(const Napi::CallbackInfo& info)
{
    auto buf = info[0].As<Napi::Uint8Array>();
    auto buf_data = buf.Data();
    auto buf_len = buf.ByteLength();

    std::vector<u8> t_buf(buf_len * 2);
	u32 size = Enc(buf_data, static_cast<int>(buf_len), t_buf.data());
	auto align_size = static_cast<std::size_t>(std::ceil(static_cast<float>(size + 16) / 16) * 16);
	std::vector<u8> y_buf(align_size);

	static const char* header = "Yaz0";
	u32 orig_size = SDL_Swap32(static_cast<u32>(buf_len));

    std::memcpy(y_buf.data(), header, 4);
    std::memcpy(y_buf.data() + 4, &orig_size, 4);
    std::memcpy(y_buf.data() + 16, t_buf.data(), size);

    return Napi::Buffer<u8>::Copy(info.Env(), y_buf.data(), y_buf.size());
}

Napi::Value Decode(const Napi::CallbackInfo& info)
{
    auto buf = info[0].As<Napi::Uint8Array>();
    auto buf_data = buf.Data();

    u32 size{};
    std::memcpy(&size, buf_data + 4, 4);
	size = SDL_Swap32(size);

	std::vector<u8> t_buf(size);
	buf_data += 16;
	Dec(buf_data, t_buf.data(), size);

    return Napi::Buffer<u8>::Copy(info.Env(), t_buf.data(), t_buf.size());
}

Napi::Object BuildExports(Napi::Env env, Napi::Object exports)
{
    exports.Set("encode", Napi::Function::New(env, Encode));
    exports.Set("decode", Napi::Function::New(env, Decode));

    return exports;
}

}
