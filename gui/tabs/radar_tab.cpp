#include "pch-il2cpp.h"
#include "radar_tab.h"
#include "gui-helpers.hpp"
#include "state.hpp"
#include "utility.h"

namespace RadarTab {
	void Render() {
		ImGui::SameLine(100 * State.dpiScale);
		ImGui::BeginChild("###Radar", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
		ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
		if (ToggleButton("显示雷达", &State.ShowRadar)) {
			State.Save();
		}

		ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
		ImGui::Separator();
		ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

		if (ToggleButton("显示尸体", &State.ShowRadar_DeadBodies)) {
			State.Save();
		}
		if (ToggleButton("显示幽灵", &State.ShowRadar_Ghosts)) {
			State.Save();
		}
		if (ToggleButton("右键传送", &State.ShowRadar_RightClickTP)) {
			State.Save();
		}

		ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
		ImGui::Separator();
		ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

		if (ToggleButton("开会期间隐藏雷达", &State.HideRadar_During_Meetings)) {
			State.Save();
		}
		if (ToggleButton("绘制玩家图标", &State.RadarDrawIcons)) {
			State.Save();
		}
		/*if (State.RadarDrawIcons && State.RevealRoles) {
			ImGui::SameLine();
			if (ToggleButton("Show Role Color on Visor", &State.RadarVisorRoleColor)) {
				State.Save();
			}
		}*/

		if (ToggleButton("锁定雷达位置", &State.LockRadar)) {
			State.Save();
		}
		if (ToggleButton("显示边框", &State.RadarBorder)) {
			State.Save();
		}
		if (ImGui::ColorEdit4("雷达颜色",
			(float*)&State.SelectedColor,
			ImGuiColorEditFlags__OptionsDefault
			| ImGuiColorEditFlags_NoInputs
			| ImGuiColorEditFlags_AlphaBar
			| ImGuiColorEditFlags_AlphaPreview)) {
			State.Save();
		}
		if (ImGui::InputInt("额外宽度", &State.RadarExtraWidth)) {
			State.RadarExtraWidth = abs(State.RadarExtraWidth); //prevent negatives
			State.Save();
		}
		if (ImGui::InputInt("额外高度", &State.RadarExtraHeight)) {
			State.RadarExtraHeight = abs(State.RadarExtraHeight); //prevent negatives
			State.Save();
		}

		ImGui::EndChild();
	}
}