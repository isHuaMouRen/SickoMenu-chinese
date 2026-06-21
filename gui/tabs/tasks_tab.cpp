#include "pch-il2cpp.h"
#include "tasks_tab.h"
#include "game.h"
#include "state.hpp"
#include "utility.h"
#include "gui-helpers.hpp"

namespace TasksTab {
	void Render() {
		if (IsInGame() && GetPlayerData(*Game::pLocalPlayer)->fields.Tasks != NULL) {
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::BeginChild("###Tasks", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
			//if (!PlayerIsImpostor(GetPlayerData(*Game::pLocalPlayer))) {
			auto tasks = GetNormalPlayerTasks(*Game::pLocalPlayer);

			bool allTasksComplete = false;
			uint16_t tasksCompleted = 0;
			for (auto task : tasks) {
				if (task->fields.taskStep == task->fields.MaxStep)
					++tasksCompleted;
			}

			if (tasks.size() != tasksCompleted) {
				if (AnimatedButton("完成所有任务")) {
					CompleteAllTasks();
				}
			}
			if (!State.SafeMode) {
				ImGui::SameLine();
			}
			if (!State.SafeMode && AnimatedButton("完成所有人的任务")) {
				for (auto player : GetAllPlayerControl()) {
					CompleteAllTasks(player);
				}
			}

			ImGui::NewLine();

			for (size_t i = 0; i < tasks.size(); ++i) {
				auto task = tasks[i];
				if (!NormalPlayerTask_get_IsComplete(task, NULL) && AnimatedButton(("完成##" + std::to_string(task->fields._._Id_k__BackingField)).c_str())) {
					State.taskRpcQueue.push(new RpcCompleteTask(task->fields._._Id_k__BackingField));
				}
				else if (NormalPlayerTask_get_IsComplete(task, NULL)) {
					ColoredButton(ImVec4(0.f, 1.f, 0.f, 1.f), ("已完成！##" + std::to_string(task->fields._._Id_k__BackingField)).c_str());
				}

				ImGui::SameLine();

				auto taskIncompleteCol = State.LightMode ? AmongUsColorToImVec4(app::Palette__TypeInfo->static_fields->Black) : AmongUsColorToImVec4(app::Palette__TypeInfo->static_fields->White);

				ImGui::TextColored(NormalPlayerTask_get_IsComplete(task, NULL)
					? ImVec4(0.0F, 1.0F, 0.0F, 1.0F)
					: taskIncompleteCol
					, TranslateTaskTypes(task->fields._.TaskType));
			}

			ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
			ImGui::Separator();
			ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
			//}

			GameOptions options;
			if (!options.GetBool(app::BoolOptionNames__Enum::VisualTasks) && ToggleButton("绕过任务不可视", &State.BypassVisualTasks))
				State.Save();

			if (options.GetGameMode() == GameModes__Enum::Normal && !options.GetBool(app::BoolOptionNames__Enum::VisualTasks)) {
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "此大厅中的任务可视已关闭.");
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "除摄像头外的所有动画均为仅客户端可见!");
			}
			else if (options.GetGameMode() == GameModes__Enum::HideNSeek)
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "在 躲猫猫 模式中，除摄像头外的动画仅客户端可见!");

			if (State.mapType == Settings::MapType::Ship) {
				if (!State.BypassVisualTasks && (options.GetGameMode() == GameModes__Enum::Normal && !options.GetBool(app::BoolOptionNames__Enum::VisualTasks)) || options.GetGameMode() == GameModes__Enum::HideNSeek) {
					if (AnimatedButton("播放盾牌动画 (仅客户端)"))
					{
						State.rpcQueue.push(new RpcPlayAnimation(1));
					}
				}
				else {
					if (AnimatedButton("播放盾牌动画"))
					{
						State.rpcQueue.push(new RpcPlayAnimation(1));
					}
				}
			}

			if (State.mapType == Settings::MapType::Ship) {
				if (!State.BypassVisualTasks && (options.GetGameMode() == GameModes__Enum::Normal && !options.GetBool(app::BoolOptionNames__Enum::VisualTasks)) || options.GetGameMode() == GameModes__Enum::HideNSeek) {
					if (AnimatedButton("播放垃圾动画 (仅客户端)"))
					{
						State.rpcQueue.push(new RpcPlayAnimation(10));
					}
				}
				else {
					if (AnimatedButton("播放垃圾动画"))
					{
						State.rpcQueue.push(new RpcPlayAnimation(10));
					}
				}
			}

			if (State.mapType == Settings::MapType::Ship || State.mapType == Settings::MapType::Pb) {

				if (!State.BypassVisualTasks && (options.GetGameMode() == GameModes__Enum::Normal && !options.GetBool(app::BoolOptionNames__Enum::VisualTasks)) || options.GetGameMode() == GameModes__Enum::HideNSeek) {
					if (ToggleButton("播放小行星武器动画 (仅客户端)", &State.PlayWeaponsAnimation))
					{
						State.Save();
					}
				}
				else {
					if (ToggleButton("播放小行星武器动画", &State.PlayWeaponsAnimation))
					{
						State.Save();
					}
				}
			}

			if (!State.BypassVisualTasks && (options.GetGameMode() == GameModes__Enum::Normal && !options.GetBool(app::BoolOptionNames__Enum::VisualTasks)) || options.GetGameMode() == GameModes__Enum::HideNSeek) {
				if (ToggleButton("播放扫描仪动画 (仅客户端)", &State.PlayMedbayScan))
				{
					if (State.PlayMedbayScan)
					{
						State.rpcQueue.push(new RpcSetScanner(true));
					}
					else
					{
						State.rpcQueue.push(new RpcSetScanner(false));
					}
				}
			}
			else {
				if (ToggleButton("播放扫描仪动画", &State.PlayMedbayScan))
				{
					if (State.PlayMedbayScan)
					{
						State.rpcQueue.push(new RpcSetScanner(true));
					}
					else
					{
						State.rpcQueue.push(new RpcSetScanner(false));
					}
				}
			}

			if (!(State.mapType == Settings::MapType::Hq || State.mapType == Settings::MapType::Fungle) && ToggleButton("假摄像头使用中", &State.FakeCameraUsage))
			{
				State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Security, (State.FakeCameraUsage ? 1 : 0)));
			}

			if (IsInMultiplayerGame() && IsInGame()) {
				float taskPercentage = (float)(*Game::pGameData)->fields.CompletedTasks / (float)(*Game::pGameData)->fields.TotalTasks;
				ImGui::TextColored(ImVec4(1.0f - taskPercentage, 1.0f, 1.0f - taskPercentage, 1.0f), "已完成 %.2f%% 项任务", taskPercentage * 100);
			}

			ImGui::EndChild();
		}
	}
}