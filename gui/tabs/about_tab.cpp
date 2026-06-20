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
#include <cstdlib>

namespace AboutTab {
    enum Groups {
        Welcome,
        Credits
    };

    static bool openWelcome = true; //default to welcome tab group
    static bool openCredits = false;

    void CloseOtherGroups(Groups group) {
        openWelcome = group == Groups::Welcome;
        openCredits = group == Groups::Credits;
    }

    const ImVec4 SickoCol = ImVec4(1.f, 0.f, 0.424f, 1.f);
    const ImVec4 RedCol = ImVec4(1.f, 0.f, 0.f, 1.0f);
    const ImVec4 AumCol = ImVec4(1.f, 0.3333f, 0.3333f, 1.0f);
    const ImVec4 GoldCol = ImVec4(1.f, 0.7333f, 0.f, 1.0f);
    const ImVec4 GoatCol = ImVec4(0.937f, 0.004f, 0.263f, 1.0f);
    const ImVec4 DevCol = ImVec4(0.102f, 0.7373f, 0.6118f, 1.0f);
    const ImVec4 ContributorCol = ImVec4(0.3804f, 0.4314f, 0.7961f, 1.0f);

    void Render() {
        ImGui::SameLine(100 * State.dpiScale);
        ImGui::BeginChild("###About", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
        if (TabGroup("欢迎", openWelcome)) {
            CloseOtherGroups(Groups::Welcome);
        }
        ImGui::SameLine();
        if (TabGroup("制作组", openCredits)) {
            CloseOtherGroups(Groups::Credits);
        }

        if (openWelcome) {
            ImGui::Text(std::format("欢迎{}", State.HasOpenedMenuBefore ? "回到 " : "来到 ").c_str());
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(SickoCol, "SickoMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(GoldCol, std::format(" {}", State.SickoVersion).c_str());
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" by ");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(GoatCol, "g0aty");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text("  |  汉化 by ");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(GoatCol, "isHuaMouRen");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text("!");

            ImGui::TextColored(SickoCol, "SickoMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" 是一款适用于《Among Us》的强大实用工具。");
            ImGui::Text("旨在提升所有玩家的游戏体验！");
            ImGui::Text("点击 \"检查更新\" 下载最新版本！");
            if (ColoredButton(DevCol, "GitHub")) {
                OpenLink("https://github.com/g0aty/SickoMenu");
            }
            ImGui::SameLine();
            if (ColoredButton(GoldCol, "检查更新")) {
                OpenLink("https://github.com/g0aty/SickoMenu/releases/latest");
            }
            ImGui::SameLine();
            if (ColoredButton(State.RgbColor, "捐赠")) {
                OpenLink("https://ko-fi.com/g0aty");
            }
            ImGui::Text("加入 Discord 服务器，获取支持、提交漏洞报告以及抢先体验！");
            if (ColoredButton(ContributorCol, "加入我们的Discord!")) {
                OpenLink("https://dsc.gg/sickos"); //SickoMenu discord invite
            }

            ImGui::TextColored(SickoCol, "SickoMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" 是一款免费且开源的软件。");

            if (State.SickoVersion.find("pr") != std::string::npos || State.SickoVersion.find("rc") != std::string::npos) {
                if (State.SickoVersion.find("pr") != std::string::npos) ImGui::TextColored(State.RgbColor, "你已获得预发行版的访问权限，祝您使用愉快！");
                else ImGui::TextColored(State.RgbColor, "您已获得发布候选版本的访问权限，祝您使用愉快！");
                BoldText("如果您无法访问我们 Discord 上的预发布版本频道，且尚未自行编译，", ImVec4(0.f, 1.f, 0.f, 1.f));
                BoldText("请在我们的 Discord 服务器上提交工单，向支持团队报告此问题！", ImVec4(0.f, 1.f, 0.f, 1.f));
            }
            else {
                ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "如果您是付费获得的此Mod，请立即申请退款。");
                BoldText("请确保您已从 GitHub 或我们的官方 Discord 下载了", ImVec4(0.f, 1.f, 0.f, 1.f));
                BoldText("最新版本的 SickoMenu！", ImVec4(0.f, 1.f, 0.f, 1.f));
            }
            //hopefully stop people from reselling a foss menu for actual money

            if (State.AprilFoolsMode) {
                ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
                auto DiddyCol = ImVec4(0.79f, 0.03f, 1.f, 1.f);
                /*ImGui::TextColored(DiddyCol, std::format("You now have access to a brand new mode: {} Mode!", State.DiddyPartyMode ? "Diddy Party" :
                (IsChatCensored() || IsStreamerMode() ? "F***son" : "Fuckson")).c_str());
                ImGui::TextColored(DiddyCol, "Find all the new features and enjoy!");
                if (ToggleButton("Diddy Party Mode", &State.DiddyPartyMode)) {
                    if (State.RizzUpEveryone) State.RizzUpEveryone = false;
                    State.Save();
                }*/
                ImGui::TextColored(DiddyCol, "愚人节快乐！");
                ImGui::TextColored(DiddyCol, "这不是真实更新，因为官方版本尚未准备就绪。");
                ImGui::TextColored(DiddyCol, "请等待官方发布，以支持《Among Us》的最新版本！");
            }
        }

        if (openCredits) {
            ImGui::TextColored(SickoCol, "SickoMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" 是基于");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(AumCol, " AmongUsMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(RedCol, " (已存档)");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" 的二次开发版本(Fork)，快去看看吧！");

            if (ColoredButton(AumCol, "AmongUsMenu")) {
                OpenLink("https://github.com/BitCrackers/AmongUsMenu");
            }
            BoldText("首席开发者", GoldCol);
            if (ColoredButton(GoatCol, "g0aty")) {
                OpenLink("https://github.com/g0aty");
            }

            BoldText("汉化版作者", GoldCol);
            if (ColoredButton(GoatCol, "isHuaMouRen")) {
                OpenLink("https://github.com/isHuaMouRen");
            }

            BoldText("开发者", DevCol);
            if (ColoredButton(DevCol, "GDjkhp")) {
                OpenLink("https://github.com/GDjkhp");
            }
            ImGui::SameLine(100.f * State.dpiScale);
            if (ColoredButton(DevCol, "Reycko")) {
                OpenLink("https://github.com/Reycko");
            }
            ImGui::SameLine(200.f * State.dpiScale);
            if (ColoredButton(DevCol, "astra1dev")) {
                OpenLink("https://github.com/astra1dev");
            }

            if (ColoredButton(DevCol, "Luckyheat")) {
                OpenLink("https://github.com/Luckyheat");
            }
            ImGui::SameLine(100.f * State.dpiScale);
            if (ColoredButton(DevCol, "UN83991956")) {
                OpenLink("https://github.com/UN83991956");
            }
            ImGui::SameLine(200.f * State.dpiScale);
            if (ColoredButton(DevCol, "HarithGamerPk")) {
                OpenLink("https://github.com/HarithGamerPk");
            }

            if (ColoredButton(DevCol, "dark-lord333")) {
                OpenLink("https://github.com/dark-lord333");
            }
            ImGui::SameLine(100.f * State.dpiScale);
            if (ColoredButton(DevCol, "WhoAboutYT")) {
                OpenLink("https://github.com/WhoAboutYT");
            }
            ImGui::SameLine(200.f * State.dpiScale);
            if (ColoredButton(DevCol, "M4-sicko")) {
                OpenLink("https://github.com/M4-sicko");
            }

            BoldText("贡献者", ContributorCol);
            if (ColoredButton(ContributorCol, "acer51-doctom")) {
                OpenLink("https://github.com/acer51-doctom");
            }
            ImGui::SameLine(100.f * State.dpiScale);
            if (ColoredButton(ContributorCol, "ZamTDS")) {
                OpenLink("https://github.com/ZamTDS");
            }

            BoldText("为AUM做出贡献的人", AumCol);
            if (ColoredButton(AumCol, "KulaGGin")) {
                OpenLink("https://github.com/KulaGGin");
            }
            ImGui::SameLine();
            ImGui::Text("(协助编写了一些用于回放系统的 ImGui 代码)");

            if (ColoredButton(AumCol, "tomsa000")) {
                OpenLink("https://github.com/tomsa000");
            }
            ImGui::SameLine();
            ImGui::Text("(协助修复内存泄漏和智能指针问题)");

            if (ColoredButton(AumCol, "cddjr")) {
                OpenLink("https://github.com/cddjr");
            }
            ImGui::SameLine();
            ImGui::Text("(协助更新至Fungle版本)");

            ImGui::Text("感谢");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(AumCol, " v0idp");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" 最初创作 ");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(AumCol, " AmongUsMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text("!");
            if (ColoredButton(AumCol, "v0idp")) {
                OpenLink("https://github.com/v0idp");
            }

            ImGui::Text("以及其他所有为");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(AumCol, " AUM ");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text("做出贡献的人我无法一一列出。");

            ImGui::Text("感谢您让 ");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(SickoCol, "SickoMenu");
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::Text(" 成为可能！");
        }
        ImGui::EndChild();
    }
}