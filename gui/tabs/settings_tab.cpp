#include "pch-il2cpp.h"
#include "settings_tab.h"
#include "utility.h"
#include "gui-helpers.hpp"
#include "state.hpp"
#include "game.h"
#include "achievements.hpp"
#include "DirectX.h"
#include "imgui/imgui_impl_win32.h" // ImGui_ImplWin32_GetDpiScaleForHwnd
#include "theme.hpp" // ApplyTheme

namespace SettingsTab {
	enum Groups {
		General,
		Spoofing,
		Customization,
		Keybinds
	};

	static bool openGeneral = true; //default to general tab group
	static bool openSpoofing = false;
	static bool openCustomization = false;
	static bool openKeybinds = false;

	void CloseOtherGroups(Groups group) {
		openGeneral = group == Groups::General;
		openSpoofing = group == Groups::Spoofing;
		openCustomization = group == Groups::Customization;
		openKeybinds = group == Groups::Keybinds;
	}

	void CheckKeybindEdit(bool hotKey) {
		State.KeybindsBeingEdited = State.KeybindsBeingEdited || hotKey;
	}

	void Render() {
		ImGui::SameLine(100 * State.dpiScale);
		ImGui::BeginChild("###Settings", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
		if (TabGroup("通用", openGeneral)) {
			CloseOtherGroups(Groups::General);
		}
		ImGui::SameLine();
		if (TabGroup("欺骗", openSpoofing)) {
			CloseOtherGroups(Groups::Spoofing);
		}
		ImGui::SameLine();
		if (TabGroup("自定义", openCustomization)) {
			CloseOtherGroups(Groups::Customization);
		}
		ImGui::SameLine();
		if (TabGroup("快捷键", openKeybinds)) {
			CloseOtherGroups(Groups::Keybinds);
		}
		if (openGeneral) {
			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
			if (ToggleButton("显示快捷键", &State.ShowKeybinds)) {
				State.Save();
			}
			ImGui::SameLine();
			if (ToggleButton("允许在聊天时激活快捷键", &State.KeybindsWhileChatting)) {
				State.Save();
			}
			if (ToggleButton("启动时始终显示菜单", &State.ShowMenuOnStartup)) {
				State.Save();
			}
			ImGui::SameLine();
			if (ToggleButton("恐慌警告", &State.PanicWarning)) {
				State.Save();
			}
			ImGui::SameLine();
			if (ToggleButton("更多命令", &State.ExtraCommands)) {
				State.Save();
			}

			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("在聊天内输入 \"/help\" 获得所有可用命令");
			}
			ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
			ImGui::Separator();
			ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

			// sorry to anyone trying to read this code it is pretty messy
#pragma region New config menu, needs fixing
			/*
			std::vector<std::string> CONFIGS = GetAllConfigs();
			CONFIGS.push_back("[New]");
			CONFIGS.push_back("[Delete]");

			std::vector<const char*> CONFIGS_CHAR;

			for (const std::string& str : CONFIGS) {
				char* ch = new char[str.size() + 1];
				std::copy(str.begin(), str.end(), ch);
				ch[str.size()] = '\0';
				CONFIGS_CHAR.push_back(ch);
			}

			bool isNewConfig = CONFIGS.size() == 1;
			bool isDelete = false;

			int& selectedConfigInt = State.selectedConfigInt;
			std::string selectedConfig = CONFIGS[selectedConfigInt];

			if (CustomListBoxInt("Configs", &selectedConfigInt, CONFIGS_CHAR), 100 * State.dpiScale, ImVec4(0,0,0,0), ImGuiComboFlags_NoArrowButton) {
				isNewConfig = selectedConfigInt == CONFIGS.size() - 2;
				isDelete = selectedConfigInt == CONFIGS.size() - 1;
				if (!isNewConfig && !isDelete) State.selectedConfig = CONFIGS[selectedConfigInt];
				State.Save();
				State.Load();
			}

			if (isNewConfig || isDelete) {
				InputString("Name", &State.selectedConfig);
				if (isNewConfig && (AnimatedButton(CheckConfigExists(State.selectedConfig) ? "Overwrite" : "Save"))) {
					State.Save();
					CONFIGS = GetAllConfigs();

					selectedConfigInt = std::distance(CONFIGS.begin(), std::find(CONFIGS.begin(), CONFIGS.end(), State.selectedConfig));
				}

				if (isDelete && CheckConfigExists(State.selectedConfig)) {
					if (AnimatedButton("Delete")) {
						selectedConfigInt--;
						State.Delete();
						CONFIGS = GetAllConfigs();
						if (selectedConfigInt < 0) selectedConfigInt = 0;
					}
				}
			}*/
#pragma endregion

			InputString("配置名称", &State.selectedConfig);

			if (CheckConfigExists(State.selectedConfig) && AnimatedButton("加载配置"))
			{
				State.SaveConfig();
				State.Load();
				State.Save(); //actually save the selected config
			}
			if (CheckConfigExists(State.selectedConfig)) ImGui::SameLine();
			if (AnimatedButton("保存配置"))
			{
				State.Save();
			}
			if (!CheckConfigExists(State.selectedConfig)) {
				ImGui::Text("配置名未找到!");
				ImGui::SameLine();
			}

			/*if (ToggleButton("Adjust by DPI", &State.AdjustByDPI)) {
				if (!State.AdjustByDPI) {
					State.dpiScale = 1.0f;
				}
				else {
					State.dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(DirectX::window);
				}
				State.dpiChanged = true;
				State.Save();
			}*/

			/*static const std::vector<const char*> DPI_SCALING_LEVEL = {"50%", "60%", "70%", "80%", "90%", "100%", "110%", "120%", "130%", "140%", "150%", "160%", "170%", "180%", "190%", "200%", "210%", "220%", "230%", "240%", "250%", "260%", "270%", "280%", "290%", "300%"};
			
			int scaleIndex = (int(std::clamp(State.dpiScale, 0.5f, 3.0f) * 100.0f) - 50) / 5;
			if (CustomListBoxInt("Menu Scale", &scaleIndex, DPI_SCALING_LEVEL, 100 * State.dpiScale)) {
				State.dpiScale = (scaleIndex * 10 + 50) / 100.0f;
				State.dpiChanged = true;
			}*/

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			if (ImGui::InputInt("FPS", &State.GameFPS)) {
				State.GameFPS = std::clamp(State.GameFPS, 1, 2147483647);
				State.Save();
			}

			ImGui::Dummy(ImVec2(1, 1) * State.dpiScale);

			if (ToggleButton("在低FPS时自动退出", &State.LeaveDueLFPS)) {
				State.Save();
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(80 * State.dpiScale);
			ImGui::InputInt("最低FPS", &State.minFpsThreshold);
			if (State.minFpsThreshold < 0)
				State.minFpsThreshold = 0;
			ImGui::PopItemWidth();

			ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);

#ifdef _DEBUG
			if (ToggleButton("显示调试标签页", &State.showDebugTab)) {
				State.Save();
			}
			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
#endif
			/*if (!IsHost() && !State.SafeMode && !IsNameValid(State.userName)) {
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.5f, 0.f, 0.f, State.MenuThemeColor.w));
				if (InputString("Username", &State.userName)) State.Save();
				ImGui::PopStyleColor();
			}
			else */if (InputString("用户名", &State.userName)) State.Save();

			if (!IsNameValid(State.userName) && !IsHost() && State.SafeMode) {
				if (State.userName == "")
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "空用户名会被反作弊系统检测到，该名称将被忽略.");
				if (State.userName.length() >= (size_t)13)
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "用户名过长，会被反作弊系统检测到。该名称将被忽略.");
				else if (!IsNameValid(State.userName))
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "用户名包含被反作弊系统屏蔽的字符。该名称将被忽略.");
				else
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "用户名被反作弊系统检测到。该名称将被忽略.");
			}

			// you can only join a lobby if you have the same name as what your requested name is, when trying to join it
			if (IsNameValid(State.userName) && (!State.SafeMode ||
				State.CurrentScene == "MatchMaking" || State.CurrentScene == "MainMenu" || State.CurrentScene == "Tutorial")) {
				if (AnimatedButton("设置账户名称")) {
					SetPlayerName(State.userName);
					LOG_INFO("成功设置账户名为 \"" + State.userName + "\"");
				}
			}

			if (/*IsNameValid(State.userName) || IsHost() || */!State.SafeMode) {
				if (IsInGame() || IsInLobby()) ImGui::SameLine();
				if ((IsInGame() || IsInLobby()) && AnimatedButton("设置名称")) {
					if (IsInGame())
						State.rpcQueue.push(new RpcSetName(State.userName));
					else if (IsInLobby())
						State.lobbyRpcQueue.push(new RpcSetName(State.userName));
					LOG_INFO("成功设置游戏内名称为 \"" + State.userName + "\"");
				}
				if (IsInGame() || IsInLobby()) ImGui::SameLine();
				if (ToggleButton("自动设置名称", &State.SetName)) {
					State.Save();
				}
			}

			if (InputString("自定义代码", &State.customCode)) {
				State.Save();
			}
			if (ToggleButton("替换主播模式大厅代码", &State.HideCode)) {
				State.Save();
			}
			ImGui::SameLine();
			if (ToggleButton("RGB 大厅代码", &State.RgbLobbyCode)) {
				State.Save();
			}

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
			ImGui::Separator();
			ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

			static float timer = 0.0f;
			static bool CosmeticsNotification = false;

			if (ToggleButton("解锁服饰", &State.UnlockCosmetics)) {
				State.Save();
				CosmeticsNotification = true;
				timer = static_cast<float>(ImGui::GetTime());
			}

			if (CosmeticsNotification) {
				float currentTime = static_cast<float>(ImGui::GetTime());
				if (currentTime - timer < 5.0f) {
					ImGui::SameLine();
					if (State.UnlockCosmetics)
						ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "服饰已解锁!");
					else
						ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "服饰已锁定!");
				}
				else {
					CosmeticsNotification = false;
				}
			}

			if (Achievements::IsSupported())
			{
				ImGui::SameLine();
				if (AnimatedButton("解锁所有成就"))
					Achievements::UnlockAll();
			}

			if (ToggleButton("允许其他模组用户看到你正在使用", &State.ModDetection)) State.Save();
			ImGui::SameLine();
			if (CustomListBoxInt(" ", &State.BroadcastedMod, MODS, 100.f * State.dpiScale)) State.Save();
		}
		if (openSpoofing) {
			/*if (ToggleButton("Spoof Guest Account", &State.SpoofGuestAccount)) {
				State.Save();
			}
			if (State.SpoofGuestAccount) {
				ImGui::SameLine();
				if (ToggleButton("Use Custom Guest Friend Code", &State.UseNewFriendCode)) {
					State.Save();
				}
				if (State.UseNewFriendCode) {
					if (InputString("Guest Friend Code", &State.NewFriendCode)) {
						State.Save();
					}
					ImGui::Text("Guest friend code should be <= 10 characters long and cannot have a hashtag.");
				}
				ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Pro Tip: You can bypass the free chat restriction using a space after your custom friend");
				ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "code!");
			}*/
			/*if (AnimatedButton("Force Login as Guest")) {
				State.ForceLoginAsGuest = true;
			}*/
			if (ToggleButton("伪造访客账户（仅限快速聊天）", &State.SpoofGuestAccount)) {
				State.Save();
			}
			if (ToggleButton("使用自定义好友代码（仅限新用户/访客账户）", &State.UseNewFriendCode)) {
				State.Save();
			}
			if (State.UseNewFriendCode) {
				ImGui::SetNextItemWidth(150 * State.dpiScale); // Adjust the width of the input box
				if (InputString("好友代码（仅限新用户/访客账户）", &State.NewFriendCode)) {
					State.Save();
				}
				ImGui::Text("这个新朋友代码长度应不超过10个字符，且不能包含空格。");
			}
			if (ToggleButton("欺骗等级", &State.SpoofLevel)) {
				State.Save();
			}
			if (State.SpoofLevel) {
				ImGui::SameLine();
				if (ImGui::InputInt("等级", &State.FakeLevel, 0, 1)) {
					State.Save();
				}

				if (State.SafeMode && (State.FakeLevel <= 0 || State.FakeLevel > 100001))
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "等级将被防作弊系统检测，您的等级将在0到100001之间.");
			}

			if (ToggleButton("欺骗平台", &State.SpoofPlatform)) {
				State.Save();
			}
			if (State.SpoofPlatform) {
				ImGui::SameLine();
				if (CustomListBoxInt("平台", &State.FakePlatform, PLATFORMS))
					State.Save();
			}

			if (State.FakePlatform == 9) {
				if (ToggleButton("伪造PSN平台ID", &State.SpoofPsnId)) {
					State.Save();
				}
				if (State.SpoofPsnId)
				{
					ImGui::SameLine();
					ImGui::SetNextItemWidth(150 * State.dpiScale);
					ImGui::InputScalar("假 PSN ID", ImGuiDataType_U64, &State.FakePsnId);

					if (AnimatedButton("随机 PSN ID")) {
						GeneratePlatformId();
					}
				}
			}

			if (State.FakePlatform == 8) {
				if (ToggleButton("欺骗 Xbox 平台 ID", &State.SpoofXboxId)) {
					State.Save();
				}
				if (State.SpoofXboxId)
				{
					ImGui::SameLine();
					ImGui::SetNextItemWidth(150 * State.dpiScale);
					ImGui::InputScalar("假 Xbox ID", ImGuiDataType_U64, &State.FakeXboxId);

					if (AnimatedButton("随机 Xbox ID")) {
						GeneratePlatformId();
					}
				}
			}

			if (ToggleButton("禁用作弊检测（主机模式）+25种模式", &State.DisableHostAnticheat)) {
				if (!State.DisableHostAnticheat && State.BattleRoyale) {
					State.BattleRoyale = false;
					State.GameMode = 0;
				}
				State.Save();
			}
			/*if (State.DisableHostAnticheat) {
				BoldText("Warning (+25 Mode)", ImVec4(1.f, 0.f, 0.f, 1.f));
				BoldText("With this option enabled, you can only find public lobbies with +25 enabled.");
				BoldText("You may not find any public lobbies in the game listing due to this.");
				BoldText("This is intended behaviour, do NOT report it as a bug.");
			}*/
			/*if (ToggleButton("Spoof Among Us Version", &State.SpoofAUVersion))
				State.Save();
			if (State.SpoofAUVersion) {
				ImGui::SameLine();
				if (CustomListBoxInt("Version", &State.FakeAUVersion, AUVERSIONS))
					State.Save();
			}*/
		}

		if (openCustomization) {
			if (ToggleButton("隐藏水印", &State.HideWatermark)) {
				State.Save();
			}

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			if (ToggleButton("亮色模式", &State.LightMode)) State.Save();
			ImGui::SameLine();
			if (!State.GradientMenuTheme) {
				if (ImGui::ColorEdit3("菜单主题色", (float*)&State.MenuThemeColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
					State.Save();
				}
			}
			else {
				if (ImGui::ColorEdit3("渐变颜色1", (float*)&State.MenuGradientColor1, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
					State.Save();
				}
				ImGui::SameLine();
				if (ImGui::ColorEdit3("渐变颜色2", (float*)&State.MenuGradientColor2, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
					State.Save();
				}
			}
			ImGui::SameLine();
			if (ToggleButton("渐变主题", &State.GradientMenuTheme))
				State.Save();

			if (ToggleButton("匹配背景与主题", &State.MatchBackgroundWithTheme)) {
				State.Save();
			}
			ImGui::SameLine();
			if (ToggleButton("RGB 菜单主题", &State.RgbMenuTheme)) {
				State.Save();
			}
			ImGui::SameLine();
			if (AnimatedButton("重置菜单主题"))
			{
				State.MenuThemeColor = ImVec4(1.f, 0.f, 0.424f, 1.f);
			}

			SteppedSliderFloat("透明度", (float*)&State.MenuThemeColor.w, 0.1f, 1.f, 0.01f, "%.2f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			if (ToggleButton("黑暗游戏主题", &State.DarkMode)) State.Save();
			ImGui::SameLine();
			if (ToggleButton("自定义游戏主题", &State.CustomGameTheme)) State.Save();

			if (State.CustomGameTheme) {
				if (ImGui::ColorEdit3("背景色", (float*)&State.GameBgColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview))
					State.Save();
				ImGui::SameLine();
				if (ImGui::ColorEdit3("文本色", (float*)&State.GameTextColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview))
					State.Save();
			}
			if (ToggleButton("更改聊天字体", &State.ChatFont)) {
				State.Save();
			}
			if (State.ChatFont) {
				ImGui::SameLine();
				if (CustomListBoxInt(" ", &State.ChatFontType, FONTS, 160.f * State.dpiScale)) {
					State.Save();
				}
			}

			ImGui::Dummy(ImVec2(4, 4)* State.dpiScale);

			if (ImGui::CollapsingHeader("GUI")) {
				ImGui::SetNextItemWidth(50 * State.dpiScale);
				if (ImGui::InputFloat("菜单缩放", &State.dpiScale)) {
					State.dpiScale = std::clamp(State.dpiScale, 0.5f, 3.f);
					State.dpiChanged = true;
					State.Save();
				}
				if (ToggleButton("禁用动画", &State.DisableAnimations))
					State.Save();
				if (ImGui::InputFloat("动画速度", &State.AnimationSpeed)) {
					if (State.AnimationSpeed <= 0) State.AnimationSpeed = 1.f;
					State.Save();
				}
				SteppedSliderFloat("圆角半径倍数", &State.RoundingRadiusMultiplier, 0.f, 2.f, 0.01f, "%.2f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);
			}

			if (ImGui::CollapsingHeader("身份颜色")) {
				ImGui::ColorEdit4("船员幽灵", (float*)&State.CrewmateGhostColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("船员", (float*)&State.CrewmateColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("工程师", (float*)&State.EngineerColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

				ImGui::ColorEdit4("天使守护者", (float*)&State.GuardianAngelColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("科学家", (float*)&State.ScientistColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("伪装者", (float*)&State.ImpostorColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				
				ImGui::ColorEdit4("变形者", (float*)&State.ShapeshifterColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("伪装者幽灵", (float*)&State.ImpostorGhostColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("大嗓门", (float*)&State.NoisemakerColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				
				ImGui::ColorEdit4("侦查者", (float*)&State.TrackerColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("幻象师", (float*)&State.PhantomColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("侦探", (float*)&State.DetectiveColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

				ImGui::ColorEdit4("毒蛇", (float*)&State.ViperColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

				if (AnimatedButton("重置身份颜色")) {
					State.CrewmateGhostColor = ImVec4(1.f, 1.f, 1.f, 0.5f);
					State.CrewmateColor = ImVec4(1.f, 1.f, 1.f, 1.f);
					State.EngineerColor = ImVec4(0.f, 1.f, 1.f, 1.f);
					State.GuardianAngelColor = ImVec4(0.5f, 0.5f, 0.5f, 0.5f);
					State.ScientistColor = ImVec4(0.2f, 0.2f, 1.f, 1.f);
					State.ImpostorColor = ImVec4(1.f, 0.1f, 0.1f, 1.f);
					State.ShapeshifterColor = ImVec4(1.f, 0.67f, 0.f, 1.f);
					State.ImpostorGhostColor = ImVec4(0.25f, 0.25f, 0.25f, 0.5f);
					State.NoisemakerColor = ImVec4(0.f, 1.f, 0.47f, 1.f);
					State.TrackerColor = ImVec4(0.65f, 0.36f, 1.f, 1.f);
					State.PhantomColor = ImVec4(0.53f, 0.f, 0.f, 1.f);
					State.DetectiveColor = ImVec4(0.39f, 0.735f, 1.f, 1.f);
					State.ViperColor = ImVec4(1.f, 1.f, 0.f, 1.f);
					State.Save();
				}
			}

			if (ImGui::CollapsingHeader("其他颜色")) {
				ImGui::ColorEdit4("大厅主持人", (float*)&State.HostColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("玩家 ID", (float*)&State.PlayerIdColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("玩家等级", (float*)&State.LevelColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

				ImGui::ColorEdit4("平台", (float*)&State.PlatformColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("是否使用Mod", (float*)&State.ModUsageColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("名称检查器", (float*)&State.NameCheckerColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

				ImGui::ColorEdit4("好友代码", (float*)&State.FriendCodeColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(150.f * State.dpiScale);
				ImGui::ColorEdit4("日期姓名", (float*)&State.DaterNamesColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SameLine(300.f * State.dpiScale);
				ImGui::ColorEdit4("大厅码", (float*)&State.LobbyCodeColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				
				ImGui::ColorEdit4("大厅年龄", (float*)&State.AgeColor, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);

				if (AnimatedButton("重置其他颜色")) {
					State.HostColor = ImVec4(1.f, 0.73f, 0.f, 1.f);
					State.PlayerIdColor = ImVec4(1.f, 0.f, 0.f, 1.f);
					State.LevelColor = ImVec4(0.f, 1.f, 0.f, 1.f);
					State.PlatformColor = ImVec4(0.73f, 0.f, 1.f, 1.f);
					State.ModUsageColor = ImVec4(1.f, 0.73f, 0.f, 1.f);
					State.NameCheckerColor = ImVec4(1.f, 0.67f, 0.f, 1.f);
					State.FriendCodeColor = ImVec4(0.2f, 0.6f, 1.f, 1.f);
					State.DaterNamesColor = ImVec4(1.f, 0.f, 0.f, 1.f);
					State.LobbyCodeColor = ImVec4(1.f, 0.73f, 0.f, 1.f);
					State.AgeColor = ImVec4(0.f, 1.f, 0.f, 1.f);
					State.Save();
				}
			}
		}

		if (openKeybinds) {
			State.KeybindsBeingEdited = false; // This should not stay on permanently

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Menu));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("显示/隐藏 菜单");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Console));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("显示/隐藏 控制台");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Radar));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("显示/隐藏 雷达");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Replay));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("显示/隐藏 回放");

			ImGui::Dummy(ImVec2(4, 4)* State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_ChatAlwaysActive));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("显示/隐藏 聊天");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_ReadGhostMessages));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("阅读幽灵聊天");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Sicko));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("恐慌模式");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Hud));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("启用/禁用 HUD");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Freecam));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("自由视角");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Zoom));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("变焦");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Noclip));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("穿墙");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Toggle_Autokill));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("自动击杀");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Reset_Appearance));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("重置外观");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Randomize_Appearance));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("立即混淆");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Repair_Sabotage));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("修复所有破坏");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Close_All_Doors));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("关闭所有门");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Close_Current_Room_Door));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("关闭当前房间门");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Complete_Tasks));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("完成所有任务");

			ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

			CheckKeybindEdit(HotKey(State.KeyBinds.Cancel_Start));
			ImGui::SameLine(100 * State.dpiScale);
			ImGui::Text("取消开始游戏");
		}
		ImGui::EndChild();
	}
}
