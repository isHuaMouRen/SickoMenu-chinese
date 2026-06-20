#include "pch-il2cpp.h"
#include "replay_tab.h"
#include "gui-helpers.hpp"
#include "state.hpp"
#include <chrono>

namespace ReplayTab {
	void Render() {
		ImGui::SameLine(100 * State.dpiScale);
		ImGui::BeginChild("###Replay", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
		ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
		if (ToggleButton("显示回放", &State.ShowReplay)) {
			State.Save();
		}
		if (ToggleButton("仅显示上次", &State.Replay_ShowOnlyLastSeconds))
		{
			State.Save();
		}
		ImGui::SameLine();
		if (SliderIntV2("秒", &State.Replay_LastSecondsValue, 1, 1200, "%d", ImGuiSliderFlags_AlwaysClamp))
		{
			State.Save();
		}

		if (ToggleButton("会议后清除", &State.Replay_ClearAfterMeeting))
		{
			State.Save();
		}

		if (ToggleButton("绘制玩家图标", &State.Replay_DrawIcons))
		{
			State.Save();
		}

		if (ImGui::ColorEdit4("回放地图颜色",
			(float*)&State.SelectedReplayMapColor,
			ImGuiColorEditFlags__OptionsDefault
			| ImGuiColorEditFlags_NoInputs
			| ImGuiColorEditFlags_AlphaBar
			| ImGuiColorEditFlags_AlphaPreview)) {
			State.Save();
		}
		ImGui::EndChild();
	}
}