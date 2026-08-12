#pragma once

namespace SelfTab {
	const std::vector<const char*> TPOPTIONS = { "无", "雷达", "任何位置" };
	const std::vector<const char*> FAKEROLES = { "船员", "伪装者", "科学家", "工程师", "守护天使", "变形者", "船员幽灵", "伪装者幽灵", "大嗓门", "幻术师", "追踪者", "侦探", "毒蛇" };
	const std::vector<const char*> NAMEGENERATION = { "单词组合", "随机字符串", "循环名称" };
	const std::vector<const char*> BODYTYPES = { "正常", "马", "长" };
	const std::vector<const char*> FONTS = { "Barlow-Italic", "Barlow-Medium", "Barlow-Bold", "Barlow-SemiBold", "Barlow-SemiBold (Masked)", "Barlow-ExtraBold", "Barlow-BoldItalic", "Barlow-BoldItalic (Masked)", "Barlow-Black", "Barlow-Light", "Barlow-Regular", "Barlow-Regular (Masked)", "Barlow-Regular (Outline)", "Brook", "LiberationSans", "NotoSans", "VCR", "CONSOLA", "digital-7", "OCRAEXT", "DIN_Pro_Bold_700" };
	//const std::vector<const char*> MATERIALS = { "Barlow-Italic Outline", "Barlow-BoldItalic Outline", "Barlow-SemiBold Outline" };
	void Render();
	void OpenSubGroup(const std::string& name);
}
