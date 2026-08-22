#include "pch-il2cpp.h"
#include "esp_tab.h"
#include "game.h"
#include "state.hpp"
#include "utility.h"
#include "gui-helpers.hpp"

namespace EspTab {

	void Render() {
		bool changed = false;
		ImGui::SameLine(100 * State.dpiScale);
		ImGui::BeginChild("###ESP", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
		changed |= ToggleButton("总开关", &State.ShowEsp);

		changed |= ToggleButton("显示幽灵", &State.ShowEsp_Ghosts);
		//dead bodies for v3.1
		changed |= ToggleButton("在开会期间隐藏", &State.HideEsp_During_Meetings);

		changed |= ToggleButton("显示方框", &State.ShowEsp_Box);
		changed |= ToggleButton("显示射线", &State.ShowEsp_Tracers);
		changed |= ToggleButton("显示距离", &State.ShowEsp_Distance);
		//better esp (from noobuild) coming v3.1
		changed |= ToggleButton("根据身份上色", &State.ShowEsp_RoleBased);

		if (State.ShowEsp_RoleBased) {
			ImGui::SameLine();
			changed |= ToggleButton("船员", &State.ShowEsp_Crew);
			ImGui::SameLine();
			changed |= ToggleButton("伪装者", &State.ShowEsp_Imp);
		}

		ImGui::EndChild();
		if (changed) {
			State.Save();
		}
	}
}