#pragma once

namespace PlayersTab {
	const std::vector<const char*> FAKEROLES = { "船员", "伪装者", "科学家", "工程师", "守护天使", "变形者", "船员幽灵", "伪装者幽灵", "大嗓门", "幻术师", "追踪者", "侦探", "毒蛇" };
	const std::vector<const char*> GHOSTROLES = { "守护天使", "船员幽灵", "伪装者幽灵" };
	const std::vector<const char*> COLORS = { "红色", "蓝色", "绿色", "粉色", "橘色", "黄色", "黑色", "白色", "紫色", "棕色", "青色", "黄绿色", "红褐色", "玫红色", "焦黄色", "灰色", "茶色", "珊瑚红", "深绿色" };
	const std::vector<const char*> SHIPVENTS = { "管理室", "走廊", "餐厅", "电力室", "上引擎", "安保室", "医务室", "武器室", "下反应堆", "下引擎", "护盾室", "上反应堆", "上导航室", "下导航室" };
	const std::vector<const char*> HQVENTS = { "Balcony", "Cafeteria", "Reactor", "Laboratory", "Office", "Admin", "Greenhouse", "Medbay", "Decontamination", "Locker Room", "Launchpad" };
	const std::vector<const char*> PBVENTS = { "Security", "Electrical", "O2", "Communications", "Office", "Admin", "Laboratory", "Lava Pool", "Storage", "Right Seismic", "Left Seismic", "Outside Admin" };
	const std::vector<const char*> AIRSHIPVENTS = { "Vault", "Cockpit", "Viewing Deck", "Engine", "Kitchen", "Lower Main Hall", "Upper Main Hall", "Right Gap Room", "Left Gap Room", "Showers", "Records", "Cargo Bay" };
	const std::vector<const char*> FUNGLEVENTS = { "Communications", "Kitchen", "Lookout", "Outside Dorm", "Laboratory", "Jungle (Laboratory)", "Jungle (Greenhouse)", "Splash Zone", "Cafeteria" };
	void Render();
}