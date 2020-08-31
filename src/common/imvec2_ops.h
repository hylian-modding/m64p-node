#pragma once

#include "imgui/imgui.h"

inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
{
	return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline ImVec2 operator+=(ImVec2& lhs, const ImVec2& rhs)
{
    lhs.x = lhs.x + rhs.x;
    lhs.y = lhs.y + rhs.y;

	return lhs;
}

inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs)
{
	return {lhs.x * rhs.x, lhs.y * rhs.y};
}

inline ImVec2 operator*=(ImVec2& lhs, const ImVec2& rhs)
{
    lhs.x = lhs.x * rhs.x;
    lhs.y = lhs.y * rhs.y;

	return lhs;
}
