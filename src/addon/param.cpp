#include "param.h"

namespace Param {

static char s_long_str[4096];
static char s_short_str[2][512];

char* GetStrUtf8(std::size_t index)
{
    if (index == 0)
        return s_long_str;
    else
        return s_short_str[index - 1];
}

void LoadStrUtf8(std::size_t index, const Napi::Value& value)
{
    std::size_t len;
    napi_get_value_string_utf8(value.Env(), static_cast<napi_value>(value),
        GetStrUtf8(index), index == 0 ? 4096 : 512, &len);
    GetStrUtf8(index)[len] = '\0';
}

void LoadStrUtf8Or(std::size_t index, const Napi::Value& value, const char* default_value)
{
    if (value.IsUndefined())
        std::strcpy(GetStrUtf8(index), default_value);
    else
        LoadStrUtf8(index, value);
}

}
