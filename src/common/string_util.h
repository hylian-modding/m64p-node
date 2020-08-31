#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace StringUtil {

std::vector<std::string> Split(const std::string& str, char delim, bool keep_delim = false);
void TrimLeftNoCopy(std::string& str);
void TrimRightNoCopy(std::string& str);
std::string Trim(std::string str);
void TrimNoCopy(std::string& str);
std::string ReadUntil(const std::string& src, std::size_t offset, char delim);
bool StartsWith(const std::string& str, std::string_view comp);
bool CaseInsCompare(const std::string& a, const std::string& b);

}
