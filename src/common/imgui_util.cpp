#include "common/imgui_util.h"
#include "imgui/imgui.h"

namespace ImGuiUtil {

void LeftLabel(const char* label, float width)
{
    auto label_w = ImGui::CalcTextSize(label).x;
	ImGui::TextUnformatted(label);
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + width - label_w);
}

}
