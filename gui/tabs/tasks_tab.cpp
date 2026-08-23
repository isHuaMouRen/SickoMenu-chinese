#include "pch-il2cpp.h"
#include "tasks_tab.h"
#include "game.h"
#include "state.hpp"
#include "utility.h"
#include "gui-helpers.hpp"

namespace TasksTab {
	using TaskList = std::vector<TaskTypes__Enum>;

	static const TaskList skeldTasks = {
		TaskTypes__Enum::SubmitScan, TaskTypes__Enum::PrimeShields, TaskTypes__Enum::FuelEngines,
		TaskTypes__Enum::ChartCourse, TaskTypes__Enum::StartReactor, TaskTypes__Enum::SwipeCard,
		TaskTypes__Enum::ClearAsteroids, TaskTypes__Enum::UploadData, TaskTypes__Enum::EmptyChute,
		TaskTypes__Enum::EmptyGarbage, TaskTypes__Enum::AlignEngineOutput, TaskTypes__Enum::FixWiring,
		TaskTypes__Enum::CalibrateDistributor, TaskTypes__Enum::DivertPower, TaskTypes__Enum::UnlockManifolds,
		TaskTypes__Enum::CleanO2Filter, TaskTypes__Enum::VentCleaning, TaskTypes__Enum::StabilizeSteering,
	};
	static const TaskList miraHqTasks = {
		TaskTypes__Enum::SubmitScan, TaskTypes__Enum::PrimeShields, TaskTypes__Enum::UploadData,
		TaskTypes__Enum::StartReactor, TaskTypes__Enum::ClearAsteroids, TaskTypes__Enum::EmptyGarbage,
		TaskTypes__Enum::FixWiring, TaskTypes__Enum::DivertPower, TaskTypes__Enum::UnlockManifolds,
		TaskTypes__Enum::VentCleaning, TaskTypes__Enum::AssembleArtifact, TaskTypes__Enum::SortSamples,
		TaskTypes__Enum::MeasureWeather, TaskTypes__Enum::EnterIdCode, TaskTypes__Enum::BuyBeverage,
		TaskTypes__Enum::ProcessData, TaskTypes__Enum::RunDiagnostics, TaskTypes__Enum::WaterPlants,
		TaskTypes__Enum::MonitorOxygen,
	};
	static const TaskList polusTasks = {
		TaskTypes__Enum::SubmitScan, TaskTypes__Enum::FuelEngines, TaskTypes__Enum::ChartCourse,
		TaskTypes__Enum::StartReactor, TaskTypes__Enum::SwipeCard, TaskTypes__Enum::ClearAsteroids,
		TaskTypes__Enum::UploadData, TaskTypes__Enum::InspectSample, TaskTypes__Enum::EmptyGarbage,
		TaskTypes__Enum::AlignEngineOutput, TaskTypes__Enum::FixWiring, TaskTypes__Enum::UnlockManifolds,
		TaskTypes__Enum::StoreArtifacts, TaskTypes__Enum::FillCanisters, TaskTypes__Enum::FixWeatherNode,
		TaskTypes__Enum::InsertKeys, TaskTypes__Enum::ScanBoardingPass, TaskTypes__Enum::OpenWaterways,
		TaskTypes__Enum::ReplaceWaterJug, TaskTypes__Enum::RepairDrill, TaskTypes__Enum::AlignTelescope,
		TaskTypes__Enum::RecordTemperature,
	};
	static const TaskList airshipTasks = {
		TaskTypes__Enum::FuelEngines, TaskTypes__Enum::UploadData, TaskTypes__Enum::EmptyChute,
		TaskTypes__Enum::EmptyGarbage, TaskTypes__Enum::FixWiring, TaskTypes__Enum::CalibrateDistributor,
		TaskTypes__Enum::DivertPower, TaskTypes__Enum::StabilizeSteering, TaskTypes__Enum::PolishRuby,
		TaskTypes__Enum::ResetBreakers, TaskTypes__Enum::Decontaminate, TaskTypes__Enum::MakeBurger,
		TaskTypes__Enum::UnlockSafe, TaskTypes__Enum::SortRecords, TaskTypes__Enum::PutAwayPistols,
		TaskTypes__Enum::FixShower, TaskTypes__Enum::CleanToilet, TaskTypes__Enum::DressMannequin,
		TaskTypes__Enum::PickUpTowels, TaskTypes__Enum::RewindTapes, TaskTypes__Enum::StartFans,
		TaskTypes__Enum::DevelopPhotos, TaskTypes__Enum::GetBiggolSword, TaskTypes__Enum::PutAwayRifles,
		TaskTypes__Enum::StopCharles, TaskTypes__Enum::VentCleaning,
	};
	static const TaskList fungleTasks = {
		TaskTypes__Enum::UploadData, TaskTypes__Enum::FixWiring, TaskTypes__Enum::VentCleaning,
		TaskTypes__Enum::RecordTemperature, TaskTypes__Enum::BuildSandcastle, TaskTypes__Enum::CatchFish,
		TaskTypes__Enum::CollectShells, TaskTypes__Enum::LiftWeights, TaskTypes__Enum::RoastMarshmallow,
		TaskTypes__Enum::TestFrisbee, TaskTypes__Enum::CollectSamples, TaskTypes__Enum::CollectVegetables,
		TaskTypes__Enum::HoistSupplies, TaskTypes__Enum::MineOres, TaskTypes__Enum::PolishGem,
		TaskTypes__Enum::ReplaceParts, TaskTypes__Enum::CrankGenerator, TaskTypes__Enum::TuneRadio,
		TaskTypes__Enum::ExtractFuel, TaskTypes__Enum::MonitorMushroom, TaskTypes__Enum::PlayVideogame,
	};

	static void RenderTaskEnforcer() {
		if (ImGui::CollapsingHeader("任务监视器", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
			if (ToggleButton("自动提出逃避任务的人", &State.AutoKickSlackers))
				State.Save();
			ImGui::SameLine();
			if (ToggleButton("忽略白名单玩家", &State.AutoKickSlackersIgnoreWhitelist))
				State.Save();
			ImGui::Text("在宽限期限后踢出低于阈值的玩家.");
			if (SliderIntV2("任务阈值 %", &State.AutoKickSlackersThreshold, 1, 100, "%d%%", ImGuiSliderFlags_NoInput))
				State.Save();
			if (SliderIntV2("宽限期 (sec)", &State.AutoKickSlackersGrace, 50, 500, "%ds", ImGuiSliderFlags_NoInput))
				State.Save();
		}
	}

	static void RenderDisableTasks() {
		if (ImGui::CollapsingHeader("禁用任务", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::TextDisabled("禁用任务将不会在下一局分配任务.");
			ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
			if (AnimatedButton("清理所有##disabletasks"))
				State.DisabledTaskTypes.clear();
			ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

			int mapId = GameOptions().GetByte(app::ByteOptionNames__Enum::MapId);
			const TaskList* currentTasks = &skeldTasks;
			const char* mapName = "The Skeld";
			switch (mapId) {
			case 1: currentTasks = &miraHqTasks; mapName = "Mira HQ"; break;
			case 2: currentTasks = &polusTasks; mapName = "Polus"; break;
			case 4: currentTasks = &airshipTasks; mapName = "Airship"; break;
			case 5: currentTasks = &fungleTasks; mapName = "Fungle"; break;
			default: break;
			}

			ImGui::TextDisabled("地图: %s", mapName);
			ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
			ImVec4 themeCol = State.RgbMenuTheme ? State.RgbColor : (State.GradientMenuTheme ? State.MenuGradientColor : State.MenuThemeColor);
			ImVec4 themeColDark = ImVec4(themeCol.x * 0.7f, themeCol.y * 0.7f, themeCol.z * 0.7f, themeCol.w);
			ImVec4 themeColDarker = ImVec4(themeCol.x * 0.5f, themeCol.y * 0.5f, themeCol.z * 0.5f, themeCol.w);

			ImGui::Columns(2, "disabledTasksCols", false);
			for (auto taskType : *currentTasks) {
				int id = (int)taskType;
				const char* name = TranslateTaskTypes(taskType);
				bool disabled = State.DisabledTaskTypes.count(id) > 0;
				ImGui::PushStyleColor(ImGuiCol_Button, disabled ? themeCol : ImVec4(0.f, 0.f, 0.f, 0.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabled ? themeColDarker : themeColDark);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, themeCol);
				if (AnimatedButton((std::string(name) + "##" + std::to_string(id)).c_str())) {
					if (disabled) State.DisabledTaskTypes.erase(id);
					else State.DisabledTaskTypes.insert(id);
				}
				ImGui::PopStyleColor(3);
				ImGui::NextColumn();
			}
			ImGui::Columns(1);
		}
	}

	void DrainHideTimer() {
		// reference: https://github.com/MrDiamond64/Hydra/blob/5962db3dbbcb44e63e8afdb20e9173f8f1ced802/src/ui/sections/TrollSection.cs#L126

		if (!GameOptions().HasOptions() || *Game::pGameData == NULL) return;

		GameOptions options;
		if (options.GetGameMode() != GameModes__Enum::HideNSeek) return;

		auto gameMgr = GameManager_get_Instance(NULL);
		auto gameFlowHns = (LogicGameFlowHnS*)gameMgr->fields._LogicFlow_k__BackingField;

		if (gameFlowHns == NULL) return;

		auto tasks = GetNormalPlayerTasks(*Game::pLocalPlayer);
		float hideTime = gameFlowHns->fields.currentHideTime;

		if (IsHost()) {
			State.rpcQueue.push(new RpcDrainHideTimer(hideTime));
			return;
		}

		if (tasks.size() == 0) return; 

		uint32_t targetTaskId = 0;
		NormalPlayerTask_TaskLength__Enum targetTaskType = NormalPlayerTask_TaskLength__Enum::None;
		for (auto task : tasks) {
			// find the longest tasks for the shortest completion count
			if (task->fields.Length <= targetTaskType) continue;

			targetTaskId = task->fields._._Id_k__BackingField;
			targetTaskType = task->fields.Length;
			if (targetTaskType == NormalPlayerTask_TaskLength__Enum::Long) break;
		}

		// using the game's logic for common/short/long task time deductions instead of using them directly
		float singleTaskTimeDeduction = 20.f - (float)GameData_get_PlayerCount(*Game::pGameData, NULL);
		if (targetTaskType != NormalPlayerTask_TaskLength__Enum::Long) singleTaskTimeDeduction /= 2.f;

		int completionCount = (int)ceil(hideTime / singleTaskTimeDeduction);

		for (int i = 0; i < completionCount; ++i) {
			State.taskRpcQueue.push(new RpcCompleteTask(targetTaskId));
		}
	}

	void Render() {
		bool inGameWithTasks = IsInGame() && GetPlayerData(*Game::pLocalPlayer)->fields.Tasks != NULL;
		bool lobbyHost = IsInLobby() && IsHost();

		if (!inGameWithTasks && !lobbyHost) return;

		ImGui::SameLine(100 * State.dpiScale);
		ImGui::BeginChild("###Tasks", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);

		if (inGameWithTasks) {
			auto tasks = GetNormalPlayerTasks(*Game::pLocalPlayer);

			uint16_t tasksCompleted = 0;
			for (auto task : tasks) {
				if (task->fields.taskStep == task->fields.MaxStep)
					++tasksCompleted;
			}

			auto gameMgr = GameManager_get_Instance(NULL);
			auto gameFlowHns = (LogicGameFlowHnS*)gameMgr->fields._LogicFlow_k__BackingField;

			if (GameOptions().HasOptions() && gameFlowHns != NULL && IsInGame() &&
				(tasks.size() != 0 || (IsHost() && gameFlowHns->fields.currentHideTime > 0.f))) {
				GameOptions options;

				if (options.GetGameMode() == GameModes__Enum::HideNSeek && AnimatedButton("直接进入终局")) {
					DrainHideTimer();
				}
				ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
			}

			if (tasks.size() != tasksCompleted) {
				if (AnimatedButton("完成所有任务")) {
					CompleteAllTasks();
				}
				ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
			}
			if (!State.SafeMode) ImGui::SameLine();
			if (!State.SafeMode && AnimatedButton("完成所有人的任务")) {
				for (auto player : GetAllPlayerControl()) {
					CompleteAllTasks(player);
				}
				ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
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

			if (tasks.size() > 0) {
				ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
				ImGui::Separator();
				ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
			}

			GameOptions options;
			if (!options.GetBool(app::BoolOptionNames__Enum::VisualTasks, false) && ToggleButton("强制开启任务可视", &State.BypassVisualTasks))
				State.Save();

			if (!State.BypassVisualTasks && options.GetGameMode() == GameModes__Enum::Normal && !options.GetBool(app::BoolOptionNames__Enum::VisualTasks, false)) {
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "此大厅已关闭任务可视.");
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "任何动画(摄像头除外)都仅客户端可见!");
			}
			else if (!State.BypassVisualTasks && options.GetGameMode() == GameModes__Enum::HideNSeek)
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "躲猫猫模式内除摄像头动画外其余动画仅客户端可见!");

			if (State.mapType == Settings::MapType::Ship) {
				if (!State.BypassVisualTasks && !options.GetBool(app::BoolOptionNames__Enum::VisualTasks, false)) {
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
				if (!State.BypassVisualTasks && !options.GetBool(app::BoolOptionNames__Enum::VisualTasks, false)) {
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

				if (!State.BypassVisualTasks && !options.GetBool(app::BoolOptionNames__Enum::VisualTasks, false)) {
					if (ToggleButton("播放武器动画 (仅客户端)", &State.PlayWeaponsAnimation))
					{
						State.Save();
					}
				}
				else {
					if (ToggleButton("播放武器动画", &State.PlayWeaponsAnimation))
					{
						State.Save();
					}
				}
			}

			if (!State.BypassVisualTasks && !options.GetBool(app::BoolOptionNames__Enum::VisualTasks, false)) {
				if (ToggleButton("播放扫描动画 (仅客户端)", &State.PlayMedbayScan))
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
				if (ToggleButton("播放扫描动画", &State.PlayMedbayScan))
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

			if (!(State.mapType == Settings::MapType::Hq || State.mapType == Settings::MapType::Fungle) && ToggleButton("播放摄像头占用动画", &State.FakeCameraUsage)) {
				State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Security, (State.FakeCameraUsage ? 1 : 0)));
			}

			if (IsInMultiplayerGame() && IsInGame()) {
				float taskPercentage = (*Game::pGameData)->fields.TotalTasks == 0 ? 1.f : (float)(*Game::pGameData)->fields.CompletedTasks / (float)(*Game::pGameData)->fields.TotalTasks;
				ImGui::TextColored(ImVec4(1.0f - taskPercentage, 1.0f, 1.0f - taskPercentage, 1.0f), "%.2f%% Total Tasks Completed", taskPercentage * 100);
			}
		}

		if (IsHost()) {
			if (inGameWithTasks) {
				ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
				ImGui::Separator();
				ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
			}
			RenderTaskEnforcer();

			if (lobbyHost) {
				ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
				RenderDisableTasks();
			}
		}

		ImGui::EndChild();
	}
}