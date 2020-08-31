#include "common/string_util.h"

#include <cctype>
#include <algorithm>

namespace StringUtil {

std::vector<std::string> Split(const std::string& src, char delim, bool keep_delim)
{
    std::vector<std::string> v;
    std::string dst;

    for (auto c : src) {
        if (c != delim || keep_delim)
            dst.push_back(c);

        if (c == delim) {
            v.push_back(dst);
            dst.clear();
        }
    }

    if (!dst.empty())
        v.push_back(dst);

    return v;
}

void TrimLeftNoCopy(std::string& str)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int c) { return !std::isspace(c); }));
}

void TrimRightNoCopy(std::string& str)
{
    str.erase(std::find_if(str.rbegin(), str.rend(), [](int c) { return !std::isspace(c); }).base(), str.end());
}

std::string Trim(std::string str)
{
    TrimLeftNoCopy(str);
    TrimRightNoCopy(str);

    return str;
}

void TrimNoCopy(std::string& str)
{
    TrimLeftNoCopy(str);
    TrimRightNoCopy(str);
}

std::string ReadUntil(const std::string& src, std::size_t offset, char delim)
{
    std::string dst;

    for (auto i = offset; i < src.size(); ++i) {
        auto c = src[i];

        if (c != delim)
            dst.push_back(c);
        else
            break;
    }

    return dst;
}

bool StartsWith(const std::string& str, std::string_view comp)
{
    return str.find(comp, 0) == 0;
}

bool CaseInsCompare(const std::string& a, const std::string& b)
{
    return a.size() == b.size()
        && std::equal(a.begin(), a.end(), b.begin(), b.end(),
        [](char c1, char c2) {
            return std::toupper(c1) == std::toupper(c2);
        });
}

}
