#pragma once
#include <vector>
#include "state.hpp"

namespace GameTab {
	const std::vector<const char*> KILL_DISTANCE = { "短", "中", "长", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20" };
	const std::vector<const char*> TASKBARUPDATES = { "总是", "开会", "从不", "3", "4", "5", "6" };
	const std::vector<const char*> COLORS = { "红色", "蓝色", "绿色", "粉色", "橘色", "黄色", "黑色", "白色", "紫色", "棕色", "青色", "黄绿色", "红褐色", "玫红色", "焦黄色", "灰色", "茶色", "珊瑚色" };
	const std::vector<const char*> HOSTCOLORS = { "红色", "蓝色", "绿色", "粉色", "橘色", "黄色", "黑色", "白色", "紫色", "棕色", "青色", "黄绿色", "红褐色", "玫红色", "焦黄色", "灰色", "茶色", "珊瑚色", "深绿色"};
	const std::vector<const char*> SMAC_PUNISHMENTS = { "什么都不做", "警告自己"/*, "Warn All (Chat)", State.SafeMode ? "Attempt to Ban" : "Attempt to Kick"*/};
	const std::vector<const char*> SMAC_HOST_PUNISHMENTS = { "什么都不做", "警告自己"/*, "Warn All (Chat)"*/, "Kick", "Ban"};
	const std::vector<const char*> SHIPVENTS = { "管理室", "走廊", "餐厅", "电力室", "上引擎室", "安保室", "医疗室", "武器室", "下反应堆", "下引擎", "护盾室", "上反应堆", "上导航室", "下导航室" };
	const std::vector<const char*> HQVENTS = { "Balcony", "Cafeteria", "Reactor", "Laboratory", "Office", "Admin", "Greenhouse", "Medbay", "Decontamination", "Locker Room", "Launchpad" };
	const std::vector<const char*> PBVENTS = { "Security", "Electrical", "O2", "Communications", "Office", "Admin", "Laboratory", "Lava Pool", "Storage", "Right Seismic", "Left Seismic", "Outside Admin" };
	const std::vector<const char*> AIRSHIPVENTS = { "Vault", "Cockpit", "Viewing Deck", "Engine", "Kitchen", "Lower Main Hall", "Upper Main Hall", "Right Gap Room", "Left Gap Room", "Showers", "Records", "Cargo Bay" };
	const std::vector<const char*> FUNGLEVENTS = { "Communications", "Kitchen", "Lookout", "Outside Dorm", "Laboratory", "Jungle (Laboratory)", "Jungle (Greenhouse)", "Splash Zone", "Cafeteria" };
	const std::vector<std::string> PLATFORM_FILTERS = { "Epic Games (PC)", "Steam (PC)", "Mac", "Microsoft Store (PC)", "itch.io (PC)", "iOS/iPadOS (Mobile)", "Android (Mobile)", "Nintendo Switch (Console)", "Xbox (Console)", "Playstation (Console)", "Unknown" };
	void Render();
	void OpenSubGroup(const std::string& name);
}
