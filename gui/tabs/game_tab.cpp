#include "pch-il2cpp.h"
#include "game_tab.h"
#include "game.h"
#include "gui-helpers.hpp"
#include "utility.h"
#include "state.hpp"
#include "logger.h"
/*#include <hunspell/hunspell.hxx>
#include <sstream>
#include <string>
#include <vector>
#include "imgui.h"

class SpellChecker {
public:
    SpellChecker(const std::string& affPath, const std::string& dicPath) {
        if (!Hunspell::isAvailable()) {
            throw std::runtime_error("Hunspell is not available.");
        }
        spell = new Hunspell(affPath.c_str(), dicPath.c_str());
        if (!spell->load()) {
            delete spell;
            throw std::runtime_error("Failed to load Hunspell dictionary.");
        }
    }

    ~SpellChecker() {
        delete spell;
    }

    bool isCorrect(const std::string& word) const {
        return spell->spell(word.c_str());
    }

private:
    Hunspell* spell;
};

void HighlightMisspelledWords(SpellChecker& checker, const std::string& text) {
    std::istringstream iss(text);
    std::string word;

    while (iss >> word) {

        bool isCorrect = checker.isCorrect(word);

        if (!isCorrect) {

            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", word.c_str());
        } else {

            ImGui::Text("%s ", word.c_str());
        }
    }
}

void RenderMenu() {
    try {
        SpellChecker spellChecker("en_US.aff", "en_US.dic");

        std::string chatMessage = "Ths is a smaple text with sme misspelled wrds.";

        if (ToggleButton("Blocked Words", &State.SMAC_CheckBadWords)) State.Save();
        if (State.SMAC_CheckBadWords) {
            HighlightMisspelledWords(spellChecker, chatMessage);

            static std::string newWord = "";
            InputString("New Word", &newWord, ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::SameLine();
            if (AnimatedButton("Add Word")) {
                State.SMAC_BadWords.push_back(newWord);
                State.Save();
                newWord = "";
            }


        }
    } catch (const std::exception& e) {

        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Error: %s", e.what());
    }
}


bool ToggleButton(const char* label, bool* p_value) {
    return ImGui::Checkbox(label, p_value);
}

void InputString(const char* label, std::string* str, int flags = 0) {
    ImGui::InputText(label, &(*str)[0], str->capacity() + 1, flags);
}


struct State {
    bool SMAC_CheckBadWords;
    void Save() {}
    static std::vector<std::string> SMAC_BadWords;
};

std::vector<std::string> State::SMAC_BadWords;

int main() {


    while (true) {
        RenderMenu();


    }

    return 0;
}

*/

static std::string strToLower(std::string str) {
    std::string new_str = "";
    for (auto i : str) {
        new_str += char(std::tolower(i));
    }
    return new_str;
}

namespace GameTab {
    enum Groups {
        General,
        Chat,
        Anticheat,
        Utils,
        History,
        Options
    };

    static bool openGeneral = true;
    static bool openChat = false;
    static bool openAnticheat = false;
    static bool openUtils = false;
        static bool openHistory = false;
    static bool openOptions = false;

    void CloseOtherGroups(Groups group) {
        openGeneral = group == Groups::General;
        openChat = group == Groups::Chat;
        openAnticheat = group == Groups::Anticheat;
        openUtils = group == Groups::Utils;
        openHistory = group == Groups::History;
        openOptions = group == Groups::Options;
    }

    void Render() {
        ImGui::SameLine(100 * State.dpiScale);
        ImGui::BeginChild("###Game", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
        if (TabGroup("通用", openGeneral)) {
            CloseOtherGroups(Groups::General);
        }
        ImGui::SameLine();
        if (TabGroup("聊天", openChat)) {
            CloseOtherGroups(Groups::Chat);
        }
        ImGui::SameLine();
        if (TabGroup("反作弊", openAnticheat)) {
            CloseOtherGroups(Groups::Anticheat);
        }
        ImGui::SameLine();
        if (TabGroup("工具", openUtils)) {
            CloseOtherGroups(Groups::Utils);
        }
        ImGui::SameLine();
        if (TabGroup("历史", openHistory)) {
            CloseOtherGroups(Groups::History);
        }

        if (GameOptions().HasOptions() && (IsInGame() || IsInLobby())) {
            ImGui::SameLine();
            if (TabGroup("选项", openOptions)) {
                CloseOtherGroups(Groups::Options);
            }
        }

        enum WarnViewType {
            WarnView_List = 0,
            WarnView_Manual,
            WarnView_COUNT
        };

        static int selectedWarnView = 0;
        const char* warnViewModes[WarnView_COUNT] = {
            "List View",
            "Manual Warn"
        };

        if (openGeneral) {
            ImGui::Dummy(ImVec2(2, 2) * State.dpiScale);
            if (SteppedSliderFloat("玩家速度倍增", &State.PlayerSpeed, 0.f, 10.f, 0.05f, "%.2fx", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput)) {
                State.PrevPlayerSpeed = State.PlayerSpeed;
            }
            if (SteppedSliderFloat("击杀距离", &State.KillDistance, 0.f, 20.f, 0.1f, "%.1f m", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput)) {
                State.PrevKillDistance = State.KillDistance;
            }
            /*if (GameOptions().GetGameMode() == GameModes__Enum::Normal) {
                if (CustomListBoxInt("Task Bar Updates", &State.TaskBarUpdates, TASKBARUPDATES, 225 * State.dpiScale))
                    State.PrevTaskBarUpdates = State.TaskBarUpdates;
            }*/
            if (ToggleButton("无技能冷却", &State.NoAbilityCD)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("应用速度倍增", &State.MultiplySpeed)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("应用击杀距离", &State.ModifyKillDistance)) {
                State.Save();
            }

            ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

            if (IsHost() || !State.SafeMode) {
                CustomListBoxInt(" ", &State.SelectedColorId, HOSTCOLORS, 85.0f * State.dpiScale);
            }
            else {
                if (State.SelectedColorId >= (int)COLORS.size()) State.SelectedColorId = 0;
                CustomListBoxInt(" ", &State.SelectedColorId, COLORS, 85.0f * State.dpiScale);
            }
            ImGui::SameLine();
            if (AnimatedButton("随机颜色"))
            {
                State.SelectedColorId = GetRandomColorId();
            }

            if (IsInGame() || IsInLobby()) {
                ImGui::SameLine();
                if (AnimatedButton("设置颜色"))
                {
                    if (IsHost() || !State.SafeMode) {
                        if (IsInGame())
                            State.rpcQueue.push(new RpcForceColor(*Game::pLocalPlayer, State.SelectedColorId));
                        else if (IsInLobby())
                            State.lobbyRpcQueue.push(new RpcForceColor(*Game::pLocalPlayer, State.SelectedColorId));
                    }
                    else if (IsColorAvailable(State.SelectedColorId)) {
                        if (IsInGame())
                            State.rpcQueue.push(new RpcSetColor(State.SelectedColorId));
                        else if (IsInLobby())
                            State.lobbyRpcQueue.push(new RpcSetColor(State.SelectedColorId));
                    }
                }
            }
            ImGui::SameLine();
            if (ToggleButton("追踪颜色", &State.SnipeColor)) {
                State.Save();
            }

            if (ToggleButton("控制台", &State.ShowConsole)) {
                State.Save();
            }

            /*if (ToggleButton("Auto-Join", &State.AutoJoinLobby))
                State.Save();
            ImGui::SameLine();
            if (InputString("Lobby Code", &State.AutoJoinLobbyCode))
                State.Save();

            if (AnimatedButton("Join Lobby")) {
                AmongUsClient_CoJoinOnlineGameFromCode(*Game::pAmongUsClient,
                    GameCode_GameNameToInt(convert_to_string(State.AutoJoinLobbyCode), NULL),
                    NULL);
            }*/

            if (IsInGame() || IsInLobby()) ImGui::SameLine();
            if ((IsInGame() || IsInLobby()) && AnimatedButton("重置外观"))
            {
                ControlAppearance(false);
            }


            if (IsInGame() && (IsHost() || !State.SafeMode) && AnimatedButton("杀死所有人")) {
                for (auto player : GetAllPlayerControl()) {
                    if (IsInGame() && (IsHost() || !State.SafeMode)) {
                        if (IsInGame())
                            State.rpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player));
                        else if (IsInLobby())
                            State.lobbyRpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player));
                    }
                    else {
                        if (IsInGame())
                            State.rpcQueue.push(new FakeMurderPlayer(*Game::pLocalPlayer, player));
                        else if (IsInLobby())
                            State.lobbyRpcQueue.push(new FakeMurderPlayer(*Game::pLocalPlayer, player));
                    }
                }
            }
            if (IsInLobby() && !State.SafeMode) ImGui::SameLine();
            if (IsInLobby() && !State.SafeMode && AnimatedButton("允许所有人穿墙")) {
                for (auto p : GetAllPlayerControl()) {
                    if (p != *Game::pLocalPlayer) State.lobbyRpcQueue.push(new RpcMurderLoop(*Game::pLocalPlayer, p, 1, true));
                }
                State.NoClip = true;
                ShowHudNotification("以允许所有人穿墙！");
            }
            /*if (IsHost() && (IsInGame() || IsInLobby()) && AnimatedButton("Spawn Dummy")) {
                auto outfit = GetPlayerOutfit(GetPlayerData(*Game::pLocalPlayer));
                if (IsInGame()) State.rpcQueue.push(new RpcSpawnDummy(outfit->fields.ColorId, convert_from_string(outfit->fields.PlayerName)));
                if (IsInLobby()) State.lobbyRpcQueue.push(new RpcSpawnDummy(outfit->fields.ColorId, convert_from_string(outfit->fields.PlayerName)));
            }*/
            if ((IsInGame() || IsInLobby()) && ((IsHost() && IsInGame()) || !State.SafeMode)) {
                ImGui::SameLine();
                if (AnimatedButton(IsHost() ? "保护所有人" : "视觉保护所有人")) {
                    for (auto player : GetAllPlayerControl()) {
                        uint8_t colorId = GetPlayerOutfit(GetPlayerData(player))->fields.ColorId;
                        if (IsInGame())
                            State.rpcQueue.push(new RpcProtectPlayer(*Game::pLocalPlayer, PlayerSelection(player), colorId));
                        else if (IsInLobby())
                            State.lobbyRpcQueue.push(new RpcProtectPlayer(*Game::pLocalPlayer, PlayerSelection(player), colorId));
                    }
                }
            }

            if (IsInGame() && ToggleButton("禁用通风管道", &State.DisableVents)) {
                State.Save();
            }
            if (IsInGame() && (IsHost() || !State.SafeMode)) ImGui::SameLine();
            if (IsInGame() && (IsHost() || !State.SafeMode) && ToggleButton("混乱报告", &State.SpamReport)) {
                State.Save();
            }

            if ((IsInGame() || (IsInLobby() && State.KillInLobbies)) && (IsHost() || !State.SafeMode)) {
                if (AnimatedButton("杀死所有船员")) {
                    for (auto player : GetAllPlayerControl()) {
                        if (!PlayerIsImpostor(GetPlayerData(player))) {
                            if (IsInGame())
                                State.rpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player));
                            else if (IsInLobby())
                                State.lobbyRpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player));
                        }
                    }
                }
                ImGui::SameLine();
                if (AnimatedButton("杀死所有伪装者")) {
                    for (auto player : GetAllPlayerControl()) {
                        if (PlayerIsImpostor(GetPlayerData(player))) {
                            if (IsInGame())
                                State.rpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player,
                                    player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                            else if (IsInLobby())
                                State.lobbyRpcQueue.push(new RpcMurderPlayer(*Game::pLocalPlayer, player,
                                    player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                        }
                    }
                }
                if (!State.SafeMode) {
                    ImGui::SameLine();
                    if (AnimatedButton("船员自杀")) {
                        for (auto player : GetAllPlayerControl()) {
                            if (!PlayerIsImpostor(GetPlayerData(player))) {
                                if (IsInGame())
                                    State.rpcQueue.push(new RpcMurderPlayer(player, player,
                                        player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                                else if (IsInLobby())
                                    State.lobbyRpcQueue.push(new RpcMurderPlayer(player, player,
                                        player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                            }
                        }
                    }
                    ImGui::SameLine();
                    if (AnimatedButton("伪装者自杀")) {
                        for (auto player : GetAllPlayerControl()) {
                            if (PlayerIsImpostor(GetPlayerData(player))) {
                                if (IsInGame())
                                    State.rpcQueue.push(new RpcMurderPlayer(player, player,
                                        player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                                else if (IsInLobby())
                                    State.lobbyRpcQueue.push(new RpcMurderPlayer(player, player,
                                        player->fields.protectedByGuardianId < 0 || State.BypassAngelProt));
                            }
                        }
                    }
                }

                static int ventId = 0;
                if (IsInGame() && (IsHost() || !State.SafeMode)) {
                    std::vector<const char*> allVents;
                    switch (State.mapType) {
                    case Settings::MapType::Ship:
                        allVents = SHIPVENTS;
                        break;
                    case Settings::MapType::Hq:
                        allVents = HQVENTS;
                        break;
                    case Settings::MapType::Pb:
                        allVents = PBVENTS;
                        break;
                    case Settings::MapType::Airship:
                        allVents = AIRSHIPVENTS;
                        break;
                    case Settings::MapType::Fungle:
                        allVents = FUNGLEVENTS;
                        break;
                    }
                    ventId = std::clamp(ventId, 0, (int)allVents.size() - 1);

                    ImGui::SetNextItemWidth(100 * State.dpiScale);
                    CustomListBoxInt("通风管道", &ventId, allVents);
                    ImGui::SameLine();
                    if (AnimatedButton("传送所有人至通风管道")) {
                        for (auto p : GetAllPlayerControl()) {
                            State.rpcQueue.push(new RpcBootFromVent(p, (State.mapType == Settings::MapType::Hq) ? ventId + 1 : ventId)); //MiraHQ vents start from 1 instead of 0
                        }
                    }
                }
            }

            if (IsInGame() || IsInLobby()) {
                bool visuals = GameOptions().GetBool(BoolOptionNames__Enum::VisualTasks);
                if (!State.SafeMode && visuals && AnimatedButton("扫描所有人")) {
                    for (auto p : GetAllPlayerControl()) {
                        if (IsInGame()) State.rpcQueue.push(new RpcForceScanner(p, true));
                        else State.lobbyRpcQueue.push(new RpcForceScanner(p, true));
                    }
                }
                if (!State.SafeMode && visuals) ImGui::SameLine();
                if (!State.SafeMode && visuals && AnimatedButton("停止扫描所有人")) {
                    for (auto p : GetAllPlayerControl()) {
                        if (IsInGame()) State.rpcQueue.push(new RpcForceScanner(p, false));
                        else State.lobbyRpcQueue.push(new RpcForceScanner(p, false));
                    }
                }
                if (IsInGame() && !State.InMeeting && !State.SafeMode && visuals) ImGui::SameLine();
                if (IsInGame() && !State.InMeeting && AnimatedButton("踢出管道内的所有人")) {
                    State.rpcQueue.push(new RpcBootAllVents());
                }
                if ((IsHost() || !State.SafeMode) && State.InMeeting) ImGui::SameLine();
                if ((IsHost() || !State.SafeMode) && State.InMeeting && AnimatedButton("结束会议")) {
                    State.rpcQueue.push(new RpcEndMeeting());
                    State.InMeeting = false;
                }

                if (!State.SafeMode && !IsHost()) {
                    if (AnimatedButton("设置所有人的名字")) {
                        for (auto p : GetAllPlayerControl()) {
                            if (IsInGame()) State.rpcQueue.push(new RpcForceName(p, std::format("{}<size=0><{}></size>", State.hostUserName, p->fields.PlayerId)));
                            if (IsInLobby()) State.lobbyRpcQueue.push(new RpcForceName(p, std::format("{}<size=0><{}></size>", State.hostUserName, p->fields.PlayerId)));
                        }
                    }
                    ImGui::SameLine();
                    if (ToggleButton("强制设置所有人的名字", &State.ForceNameForEveryone)) {
                        State.Save();
                    }

                    if (InputString("用户名", &State.hostUserName)) {
                        State.Save();
                    }

                    if (AnimatedButton("设置所有人的颜色")) {
                        for (auto p : GetAllPlayerControl()) {
                            if (IsInGame()) State.rpcQueue.push(new RpcForceColor(p, State.HostSelectedColorId));
                            if (IsInLobby()) State.lobbyRpcQueue.push(new RpcForceColor(p, State.HostSelectedColorId));
                        }
                    }
                    ImGui::SameLine();
                    if (ToggleButton("强制设置所有人的颜色", &State.ForceColorForEveryone)) {
                        State.Save();
                    }

                    if (CustomListBoxInt(" ­", &State.HostSelectedColorId, HOSTCOLORS, 85.0f * State.dpiScale)) State.Save();
                }
            }
        }

        if (openChat) {
            bool msgAllowed = IsChatValid(State.chatMessage);
            if (!msgAllowed) {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.5f, 0.f, 0.f, State.MenuThemeColor.w));
                if (InputStringMultiline("\n\n\n\n\n聊天内容", &State.chatMessage)) State.Save();
                ImGui::PopStyleColor();
            }
            else if (InputStringMultiline("\n\n\n\n\n聊天内容", &State.chatMessage)) State.Save();
            if ((IsInGame() || IsInLobby()) && State.ChatCooldown >= 3.f && IsChatValid(State.chatMessage)) {
                ImGui::SameLine();
                if (AnimatedButton("发送"))
                {
                    auto player = (!State.SafeMode && State.playerToChatAs.has_value()) ?
                        State.playerToChatAs.validate().get_PlayerControl() : *Game::pLocalPlayer;
                    if (IsInGame()) State.rpcQueue.push(new RpcSendChat(player, State.chatMessage));
                    else if (IsInLobby()) State.lobbyRpcQueue.push(new RpcSendChat(player, State.chatMessage));
                    State.MessageSent = true;
                }
            }
            if ((IsInGame() || IsInLobby()) && State.ReadAndSendSickoChat) ImGui::SameLine();
            if (State.ReadAndSendSickoChat && (IsInGame() || IsInLobby()) && AnimatedButton("发送 SickoChat"))
            {
                auto player = (!State.SafeMode && State.playerToChatAs.has_value()) ?
                    State.playerToChatAs.validate().get_PlayerControl() : *Game::pLocalPlayer;
                if (IsInGame()) {
                    State.rpcQueue.push(new RpcForceSickoChat(PlayerSelection(player), State.chatMessage, true));
                }
                else if (IsInLobby()) {
                    State.lobbyRpcQueue.push(new RpcForceSickoChat(PlayerSelection(player), State.chatMessage, true));
                }
            }

            if (ToggleButton("开启轰炸", &State.ChatSpam))
            {
                if (State.BrainrotEveryone) State.BrainrotEveryone = false;
                if (State.RizzUpEveryone) State.RizzUpEveryone = false;
                State.Save();
            }
            if (((IsHost() && IsInGame()) || !State.SafeMode) && State.ChatSpamMode) ImGui::SameLine();
            if ((IsHost() || !State.SafeMode) && State.ChatSpamMode && ToggleButton("全员参与轰炸", &State.ChatSpamEveryone))
            {
                State.Save();
            }
            if (IsHost() || !State.SafeMode) {
                if (CustomListBoxInt("文本轰炸", &State.ChatSpamMode,
                    { State.SafeMode ? "仅带文本(仅自己轰炸)" : "带文本", "空白聊天", State.SafeMode ? "自身文本 + 空白聊天" : "文本 + 空白聊天" })) State.Save();
            }

            if (std::find(State.ChatPresets.begin(), State.ChatPresets.end(), State.chatMessage) == State.ChatPresets.end() && AnimatedButton("添加消息到预设")) {
                State.ChatPresets.push_back(State.chatMessage);
                State.Save();
            }
            if (!(IsHost() || !State.SafeMode) && State.chatMessage.size() > 120) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "消息将被反作弊系统检测到.");
            }
            if (!State.ChatPresets.empty()) {
                static int selectedPresetIndex = 0;
                selectedPresetIndex = std::clamp(selectedPresetIndex, 0, (int)State.ChatPresets.size() - 1);
                std::vector<const char*> presetVector(State.ChatPresets.size(), nullptr);
                for (size_t i = 0; i < State.ChatPresets.size(); i++) {
                    presetVector[i] = State.ChatPresets[i].c_str();
                }
                CustomListBoxInt("待发送/删除的消息", &selectedPresetIndex, presetVector);
                auto msg = State.ChatPresets[selectedPresetIndex];
                if (AnimatedButton("设置为聊天消息"))
                {
                    State.chatMessage = msg;
                }
                ImGui::SameLine();
                if (AnimatedButton("移除"))
                    State.ChatPresets.erase(State.ChatPresets.begin() + selectedPresetIndex);
            }
        }

        if (openAnticheat) {
            if (ToggleButton("启用反作弊 (SMAC)", &State.Enable_SMAC)) State.Save();
            if (IsHost()) CustomListBoxInt("主持人惩罚 ", &State.SMAC_HostPunishment, SMAC_HOST_PUNISHMENTS, 85.0f * State.dpiScale);
            else CustomListBoxInt("普通惩罚", &State.SMAC_Punishment, SMAC_PUNISHMENTS, 85.0f * State.dpiScale);

            if (ToggleButton("添加作弊者到黑名单", &State.SMAC_AddToBlacklist)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("惩罚黑名单", &State.SMAC_PunishBlacklist)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("忽略白名单", &State.SMAC_IgnoreWhitelist)) State.Save();
            if (State.SMAC_PunishBlacklist) {
                ImGui::Text("黑名单");
                if (State.BlacklistFriendCodes.empty())
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "没有用户在黑名单内！");
                else {
                    ImGui::SameLine(0.f, 0.f);
                    ImGui::Text(" (%d 已添加进黑名单)", State.BlacklistFriendCodes.size());
                }
                static std::string newBFriendCode = "";
                bool isInBlacklistAlready = std::find(State.BlacklistFriendCodes.begin(), State.BlacklistFriendCodes.end(), newBFriendCode) != State.BlacklistFriendCodes.end();
                InputString("新好友代码", &newBFriendCode, ImGuiInputTextFlags_EnterReturnsTrue);
                if (isInBlacklistAlready)
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "此用户已在黑名单内!");
                if (newBFriendCode != "" && !isInBlacklistAlready) ImGui::SameLine();
                if (newBFriendCode != "" && !isInBlacklistAlready && AnimatedButton("添加")) {
                    State.BlacklistFriendCodes.push_back(newBFriendCode);
                    State.Save();
                    newBFriendCode = "";
                }

                if (!State.BlacklistFriendCodes.empty()) {
                    static int selectedBCodeIndex = 0;
                    selectedBCodeIndex = std::clamp(selectedBCodeIndex, 0, (int)State.BlacklistFriendCodes.size() - 1);
                    std::vector<const char*> bCodeVector(State.BlacklistFriendCodes.size(), nullptr);
                    for (size_t i = 0; i < State.BlacklistFriendCodes.size(); i++) {
                        bCodeVector[i] = State.BlacklistFriendCodes[i].c_str();
                    }
                    CustomListBoxInt("要删除的玩家", &selectedBCodeIndex, bCodeVector);
                    ImGui::SameLine();
                    if (AnimatedButton("删除"))
                        State.BlacklistFriendCodes.erase(State.BlacklistFriendCodes.begin() + selectedBCodeIndex);
                }
            }
            if (State.SMAC_IgnoreWhitelist) {
                ImGui::Text("白名单");
                if (State.WhitelistFriendCodes.empty())
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "没有用户在白名单内!");
                else {
                    ImGui::SameLine(0.f, 0.f);
                    ImGui::Text(" (%d 已添加进白名单)", State.WhitelistFriendCodes.size());
                }
                static std::string newWFriendCode = "";
                static bool isInWhitelistAlready = std::find(State.WhitelistFriendCodes.begin(), State.WhitelistFriendCodes.end(), newWFriendCode) != State.WhitelistFriendCodes.end();
                InputString("新好友代码\n", &newWFriendCode, ImGuiInputTextFlags_EnterReturnsTrue);
                if (isInWhitelistAlready)
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "此用户已在白名单内!");
                if (newWFriendCode != "" && !isInWhitelistAlready) ImGui::SameLine();
                if (newWFriendCode != "" && !isInWhitelistAlready && AnimatedButton("添加\n")) {
                    State.WhitelistFriendCodes.push_back(newWFriendCode);
                    State.Save();
                    newWFriendCode = "";
                }

                if (!State.WhitelistFriendCodes.empty()) {
                    static int selectedWCodeIndex = 0;
                    selectedWCodeIndex = std::clamp(selectedWCodeIndex, 0, (int)State.WhitelistFriendCodes.size() - 1);
                    std::vector<const char*> wCodeVector(State.WhitelistFriendCodes.size(), nullptr);
                    for (size_t i = 0; i < State.WhitelistFriendCodes.size(); i++) {
                        wCodeVector[i] = State.WhitelistFriendCodes[i].c_str();
                    }
                    CustomListBoxInt("要删除的玩家\n", &selectedWCodeIndex, wCodeVector);
                    ImGui::SameLine();
                    if (AnimatedButton("删除"))
                        State.WhitelistFriendCodes.erase(State.WhitelistFriendCodes.begin() + selectedWCodeIndex);
                }
            }
            ImGui::Text("检测操作:");
            if (ToggleButton("使用 AUM/KillNetwork", &State.SMAC_CheckAUM)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("使用 SickoMenu", &State.SMAC_CheckSicko)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("异常名称", &State.SMAC_CheckBadNames)) State.Save();

            if (ToggleButton("异常设置颜色", &State.SMAC_CheckColor)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("异常设置服饰", &State.SMAC_CheckCosmetics)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("异常聊天", &State.SMAC_CheckChatNote)) State.Save();

            if (ToggleButton("异常扫描器", &State.SMAC_CheckScanner)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("异常动画", &State.SMAC_CheckAnimation)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("设置任务", &State.SMAC_CheckTasks)) State.Save();

            if (ToggleButton("异常击杀", &State.SMAC_CheckMurder)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("异常变形", &State.SMAC_CheckShapeshift)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("异常隐身", &State.SMAC_CheckVanish)) State.Save();


            if (ToggleButton("异常会议/尸体报告", &State.SMAC_CheckReport)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("异常通风管道", &State.SMAC_CheckVent)) State.Save();
            ImGui::SameLine();
           
            if (ToggleButton("异常聊天", &State.SMAC_CheckChat)) State.Save();

            if (ToggleButton("异常任务完成", &State.SMAC_CheckTaskCompletion)) State.Save();
            ImGui::SameLine();
            if (ToggleButton("异常破坏", &State.SMAC_CheckSabotage)) State.Save();
            if (ToggleButton("异常玩家等级 (0表示忽略)", &State.SMAC_CheckLevel)) State.Save();
            if (State.SMAC_CheckLevel && ImGui::InputInt("等级 >=", &State.SMAC_HighLevel)) {
                State.Save();
            }
            if (State.SMAC_CheckLevel && ImGui::InputInt("等级 <=", &State.SMAC_LowLevel)) {
                State.Save();
            }
            if (ToggleButton("屏蔽词汇", &State.SMAC_CheckBadWords)) State.Save();
            if (State.SMAC_CheckBadWords) {
                if (State.SMAC_BadWords.empty())
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "无不当词汇添加!");
                static std::string newWord = "";
                InputString("新词汇", &newWord, ImGuiInputTextFlags_EnterReturnsTrue);
                ImGui::SameLine();
                if (AnimatedButton("添加词汇")) {
                    State.SMAC_BadWords.push_back(newWord);
                    State.Save();
                    newWord = "";
                }
                if (!State.SMAC_BadWords.empty()) {
                    static int selectedWordIndex = 0;
                    selectedWordIndex = std::clamp(selectedWordIndex, 0, (int)State.SMAC_BadWords.size() - 1);
                    std::vector<const char*> wordVector(State.SMAC_BadWords.size(), nullptr);
                    for (size_t i = 0; i < State.SMAC_BadWords.size(); i++) {
                        wordVector[i] = State.SMAC_BadWords[i].c_str();
                    }
                    CustomListBoxInt("要移除的词汇", &selectedWordIndex, wordVector);
                    ImGui::SameLine();
                    if (AnimatedButton("移除"))
                        State.SMAC_BadWords.erase(State.SMAC_BadWords.begin() + selectedWordIndex);
                }
            }
        }

        if (openUtils) {
            /*if (ToggleButton("Ignore Whitelisted Players [Exploits]", &State.Destruct_IgnoreWhitelist)) {
                State.Save();
            }*/
            if (ToggleButton("忽略白名单玩家的踢出/封禁", &State.Ban_IgnoreWhitelist)) {
                State.Save();
            }
            if (IsInLobby() && ToggleButton("尝试崩溃大厅", &State.CrashSpamReport)) {
                State.Save();
            }
            if (State.CrashSpamReport) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ("当游戏开始时，大厅已被摧毁。"));
            if (State.AprilFoolsMode) {
                ImGui::TextColored(ImVec4(0.79f, 0.03f, 1.f, 1.f), State.DiddyPartyMode ? "Diddy 派对模式" : (IsChatCensored() || IsStreamerMode() ? "F***son 模式" : "Fuckson 模式"));
                if (ToggleButton("Mog Everyone [Sigma]", &State.BrainrotEveryone)) {
                    if (State.ChatSpam) State.ChatSpam = false;
                    if (State.RizzUpEveryone) State.RizzUpEveryone = false;
                    State.Save();
                }
                if (/*State.DiddyPartyMode && */ToggleButton("让每个人变得更强 [Skibidi]", &State.RizzUpEveryone)) {
                    if (State.ChatSpam) State.ChatSpam = false;
                    if (State.BrainrotEveryone) State.BrainrotEveryone = false;
                    State.Save();
                }
            }
            if (IsHost()) {
                ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
                if (((IsInGame() && Object_1_IsNotNull((Object_1*)*Game::pShipStatus)) || (IsInLobby() && Object_1_IsNotNull((Object_1*)*Game::pLobbyBehaviour)))
                    && AnimatedButton(IsInLobby() ? "移除大厅" : "移除地图")) {
                    State.taskRpcQueue.push(new DestroyMap());
                }
                ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
                if (ToggleButton("封禁所有人", &State.BanEveryone)) {
                    State.Save();
                }
                if (ToggleButton("踢出所有人", &State.KickEveryone)) {
                    State.Save();
                }
                if (SteppedSliderFloat("踢出/封禁 延迟", &State.AutoPunishDelay, 0.f, 10.f, 0.1f, "%.1f", ImGuiSliderFlags_NoInput)) {
                    State.Save();
                }
                ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
                const char* buttonLabel = IsInGame() ? "提出挂机玩家" : "提出挂机玩家 (仅游戏中)";
                if (ToggleButton(buttonLabel, &State.KickAFK)) {
                    State.Save();
                }
                if (State.KickAFK) ImGui::SameLine();
                if (State.KickAFK && ToggleButton("启用挂机提示", &State.NotificationsAFK)) {
                    State.Save();
                }
                if (State.KickAFK && ToggleButton("挂机 - 第二次机会", &State.SecondChance)) {
                    State.Save();
                }
                std::string header = "反挂机 ~ 高级选项";
                if (!IsInGame()) {
                    header += " [游戏比赛]";
                }
                ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
                if (State.KickAFK && ImGui::CollapsingHeader(header.c_str()))
                {
                    if (SteppedSliderFloat("踢出前的时间", &State.TimerAFK, 40.f, 350.f, 1.f, "%.0f", ImGuiSliderFlags_NoInput)) {
                        State.Save();
                    }
                    if (State.SecondChance && SteppedSliderFloat("更多时间", &State.AddExtraTime, 15.f, 120.f, 1.f, "%.0f", ImGuiSliderFlags_NoInput)) {
                        State.Save();
                    }
                    if (State.SecondChance && SteppedSliderFloat("添加前的剩余时间", &State.ExtraTimeThreshold, 5.f, 60.f, 1.f, "%.0f", ImGuiSliderFlags_NoInput)) {
                        State.Save();
                    }
                    if (State.NotificationsAFK && SteppedSliderFloat("挂机通知时间", &State.NotificationTimeWarn, 5.f, 60.f, 1.f, "%.0f", ImGuiSliderFlags_NoInput)) {
                        State.Save();
                    }
                }
                ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                ImGui::Separator();
                ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                if (ToggleButton("仅白名单玩家", &State.KickByWhitelist)) {
                    State.Save();
                }
                if (State.KickByWhitelist) ImGui::SameLine();
                if (State.KickByWhitelist && ToggleButton("启用 WL 通知", &State.WhitelistNotifications)) {
                    State.Save();
                }
                ImGui::Dummy(ImVec2(15, 15) * State.dpiScale);
                if (ToggleButton("封禁自动重连玩家", &State.BanLeavers)) {
                    State.Save();
                }
                ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
                if (ImGui::CollapsingHeader("BA-RP ~ 高级选项"))
                {
                    if (SteppedSliderFloat("最多重连", &State.LeaveCount, 1.f, 15.f, 1.f, "%.0f", ImGuiSliderFlags_NoInput)) {
                        State.Save();
                    }
                    ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
                    if (ToggleButton("自动重连黑名单玩家", &State.BL_AutoLeavers)) {
                        State.Save();
                    }
                }
                ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                ImGui::Separator();
                ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                if (ToggleButton("按名字检查警告/踢出", &State.KickByLockedName)) {
                    State.Save();
                }
                if (State.KickByLockedName) ImGui::SameLine();
                if (State.KickByLockedName && ToggleButton("显示玩家数据通知", &State.ShowPDataByNC)) {
                    State.Save();
                }
                if (State.KickByLockedName) {
                    ImGui::Text("被屏蔽的名称");
                    if (State.LockedNames.empty())
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "名称检查器中没有用户!");
                    static std::string newName = "";
                    InputString("新昵称", &newName, ImGuiInputTextFlags_EnterReturnsTrue);
                    if (newName != "") ImGui::SameLine();
                    if (newName != "" && AnimatedButton("添加")) {
                        newName = strToLower(newName);
                        State.LockedNames.push_back(newName);
                        State.Save();
                        newName = "";
                    }

                    if (!State.LockedNames.empty()) {
                        static int selectedName = 0;
                        selectedName = std::clamp(selectedName, 0, (int)State.LockedNames.size() - 1);
                        std::vector<const char*> bNameVector(State.LockedNames.size(), nullptr);
                        for (size_t i = 0; i < State.LockedNames.size(); i++) {
                            bNameVector[i] = State.LockedNames[i].c_str();
                        }
                        CustomListBoxInt("要删除的昵称", &selectedName, bNameVector);
                        ImGui::SameLine();
                        if (AnimatedButton("删除"))
                            State.LockedNames.erase(State.LockedNames.begin() + selectedName);
                    }
                }
                ImGui::Dummy(ImVec2(15, 15) * State.dpiScale);
                ImGui::BeginGroup();
                if (ToggleButton("踢出警告玩家", &State.KickWarned)) {
                    State.Save();
                }
                if (ToggleButton("封禁警告玩家", &State.BanWarned)) {
                    State.Save();
                }
                if (ToggleButton("通知警告玩家", &State.NotifyWarned)) {
                    State.Save();
                }

                ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);

                ImGui::PushItemWidth(80);
                ImGui::InputInt("最大警告", &State.MaxWarns);
                if (State.MaxWarns < 1)
                    State.MaxWarns = 1;
                ImGui::PopItemWidth();
                ImGui::EndGroup();
            }
            if (IsHost()) ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::PushItemWidth(150);
            if (!IsHost()) ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            ImGui::Combo("警告视图模式", &selectedWarnView, warnViewModes, WarnView_COUNT);
            ImGui::PopItemWidth();


            if (selectedWarnView == WarnView_List) {
                if (!State.WarnedFriendCodes.empty()) {
                    ImGui::Text("已警告玩家");

                    std::string localFC = "";
                    if (Game::pLocalPlayer && *Game::pLocalPlayer) {
                        localFC = convert_from_string((*Game::pLocalPlayer)->fields.FriendCode);
                    }

                    std::vector<std::string> warnedList;
                    std::vector<std::string> fcKeys;

                    for (const auto& [fc, count] : State.WarnedFriendCodes) {
                        if (count <= 0 || fc == localFC)
                            continue;

                        warnedList.push_back(std::format("{} ({} warn{})", fc, count, count == 1 ? "" : "s"));
                        fcKeys.push_back(fc);
                    }

                    if (!warnedList.empty()) {
                        static int selectedWarned = 0;
                        selectedWarned = std::clamp(selectedWarned, 0, (int)warnedList.size() - 1);

                        std::vector<const char*> warnedCStrs;
                        for (const auto& entry : warnedList) warnedCStrs.push_back(entry.c_str());

                        ImGui::PushItemWidth(200);
                        CustomListBoxInt("已警告好友代码", &selectedWarned, warnedCStrs);
                        ImGui::PopItemWidth();

                        ImGui::SameLine();
                        if (ImGui::Button("移除")) {
                            if (selectedWarned >= 0 && selectedWarned < (int)fcKeys.size()) {
                                std::string fc = fcKeys[selectedWarned];
                                State.WarnedFriendCodes.erase(fc);
                                State.WarnReasons.erase(fc);
                                selectedWarned = 0;
                                State.Save();
                            }
                        }

                        std::string selectedFc = fcKeys[selectedWarned];
                        auto& warnReasons = State.WarnReasons[selectedFc];

                        if (!warnReasons.empty()) {
                            ImGui::Text("警告原因:");

                            static int selectedReason = 0;
                            selectedReason = std::clamp(selectedReason, 0, (int)warnReasons.size() - 1);

                            std::vector<std::string> numberedReasons;
                            numberedReasons.reserve(warnReasons.size());
                            for (size_t i = 0; i < warnReasons.size(); ++i) {
                                numberedReasons.push_back(std::format("[{}] {}", i + 1, warnReasons[i]));
                            }

                            std::vector<const char*> reasonCStrs;
                            for (const auto& str : numberedReasons) reasonCStrs.push_back(str.c_str());

                            ImGui::PushItemWidth(200);
                            ImGui::ListBox("##WarnReasonList", &selectedReason, reasonCStrs.data(), (int)reasonCStrs.size());
                            ImGui::PopItemWidth();

                            ImGui::SameLine();
                            if (ImGui::Button("移除")) {
                                if (selectedReason >= 0 && selectedReason < (int)warnReasons.size()) {
                                    warnReasons.erase(warnReasons.begin() + selectedReason);
                                    selectedReason = 0;

                                    if (--State.WarnedFriendCodes[selectedFc] <= 0) {
                                        State.WarnedFriendCodes.erase(selectedFc);
                                        State.WarnReasons.erase(selectedFc);
                                        selectedWarned = 0;
                                    }

                                    State.Save();
                                }
                            }
                        }
                    }
                    else {
                        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "没有被警告的玩家");
                    }
                }
            }
            else if (selectedWarnView == WarnView_Manual) {
                static std::string friendCodeToWarn;
                static std::string warnReason;

                ImGui::PushItemWidth(200);
                InputString("FriendCode##warn", &friendCodeToWarn);
                InputString("原因", &warnReason);
                ImGui::PopItemWidth();

                if (ImGui::Button("提交警告") && !friendCodeToWarn.empty() && !warnReason.empty()) {
                    State.WarnedFriendCodes[friendCodeToWarn]++;
                    State.WarnReasons[friendCodeToWarn].push_back(warnReason);
                    State.Save();

                    friendCodeToWarn.clear();
                    warnReason.clear();
                }
            }

            ImGui::EndGroup();

            ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

            if (ToggleButton("启用临时封禁系统", &State.TempBanEnabled)) {
                State.Save();
            }
            if (State.TempBanEnabled && ImGui::CollapsingHeader("临时封禁系统")) {
                static std::string friendCodeToTempBan;
                static int banDays = 0, banHours = 0, banMinutes = 0, banSeconds = 0;

                ImGui::BeginGroup();
                ImGui::PushItemWidth(150);
                InputString("好友代码", &friendCodeToTempBan);

                ImGui::InputInt("日", &banDays);     banDays = std::max<int>(0, banDays);
                ImGui::InputInt("小时", &banHours);   banHours = std::clamp(banHours, 0, 23);
                ImGui::InputInt("分钟", &banMinutes); banMinutes = std::clamp(banMinutes, 0, 59);
                ImGui::InputInt("秒", &banSeconds); banSeconds = std::clamp(banSeconds, 0, 59);
                ImGui::PopItemWidth();

                if (!friendCodeToTempBan.empty() && ImGui::Button("提交临时封禁")) {
                    std::string selfFC;
                    if (Game::pLocalPlayer && *Game::pLocalPlayer) {
                        selfFC = convert_from_string((*Game::pLocalPlayer)->fields.FriendCode);
                    }

                    if (!selfFC.empty() && friendCodeToTempBan == selfFC) { }
                    else {
                        int64_t totalSeconds = 0;
                        totalSeconds += static_cast<int64_t>(banDays) * 86400;
                        totalSeconds += static_cast<int64_t>(banHours) * 3600;
                        totalSeconds += static_cast<int64_t>(banMinutes) * 60;
                        totalSeconds += static_cast<int64_t>(banSeconds);

                        if (totalSeconds > State.MAX_BAN_SECONDS) {
                            totalSeconds = State.MAX_BAN_SECONDS;
                        }

                        if (totalSeconds > 0) {
                            auto now = std::chrono::system_clock::now();
                            auto banEnd = now + std::chrono::seconds(totalSeconds);

                            State.TempBannedFCs[friendCodeToTempBan] = banEnd;
                            State.Save();

                            if (IsInGame() || IsInLobby()) {
                                for (auto p : GetAllPlayerControl()) {
                                    if (!p) continue;
                                    if (convert_from_string(p->fields.FriendCode) == friendCodeToTempBan) {
                                        // Main & first ban (new temp-banned user):
                                        if (IsInGame())
                                            State.rpcQueue.push(new PunishPlayer(p, false));
                                        if (IsInLobby())
                                            State.lobbyRpcQueue.push(new PunishPlayer(p, false));
                                    }
                                }
                            }
                        }
                    }
                }
                ImGui::EndGroup();
                ImGui::SameLine();

                ImGui::BeginGroup();
                ImGui::Text("临时封禁玩家:");

                auto now = std::chrono::system_clock::now();
                if (State.TempBannedFCs.empty()) {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "没有玩家被临时封禁.");
                }
                else {
                    static int selectedTempBanIndex = 0;
                    std::vector<std::string> displayList, friendCodeList;

                    for (const auto& [fc, until] : State.TempBannedFCs) {
                        auto timeLeft = std::chrono::duration_cast<std::chrono::seconds>(until - now).count();
                        if (timeLeft < 0) timeLeft = 0;

                        int d = (int)(timeLeft / 86400);
                        int h = (int)((timeLeft % 86400) / 3600);
                        int m = (int)((timeLeft % 3600) / 60);
                        int s = (int)(timeLeft % 60);

                        char buffer[128];
                        snprintf(buffer, sizeof(buffer), "%s | %02dd:%02dh:%02dm:%02ds", fc.c_str(), d, h, m, s);

                        displayList.push_back(buffer);
                        friendCodeList.push_back(fc);
                    }

                    std::vector<const char*> displayCStrs;
                    for (auto& s : displayList) displayCStrs.push_back(s.c_str());

                    selectedTempBanIndex = std::clamp(selectedTempBanIndex, 0, (int)displayCStrs.size() - 1);
                    CustomListBoxInt("选择临时封禁", &selectedTempBanIndex, displayCStrs);

                    if (ImGui::Button("解封")) {
                        if (selectedTempBanIndex >= 0 && selectedTempBanIndex < (int)friendCodeList.size()) {
                            std::string targetFC = friendCodeList[selectedTempBanIndex];
                            State.TempBannedFCs.erase(targetFC);
                            State.Save();
                        }
                    }
                }

                ImGui::Dummy(ImVec2(10, 10) * State.dpiScale);
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "注意: 临时封禁仅主持人可用");
                ImGui::EndGroup();
            }
        }

       if (openHistory) {
            ImGui::Text("最后100位玩家:");

            static std::string historySearchBuf = "";
ImGui::SetNextItemWidth(200);
InputString("##HistorySearch", &historySearchBuf);
            ImGui::SameLine();
            ImGui::TextDisabled("搜索");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("过滤名字, 好友代码, 或 PUID");
           
           static int selectedIndex = -1;

           static std::string lastSearchQuery = "";
if (historySearchBuf != lastSearchQuery) {
    lastSearchQuery = historySearchBuf;
    selectedIndex = -1;
}

            std::vector<std::string> decoratedStorage;
            decoratedStorage.reserve(State.PlayerHistory.size());
            std::vector<const char*> names;
            std::vector<int> filteredIndices;

            names.reserve(State.PlayerHistory.size());
            filteredIndices.reserve(State.PlayerHistory.size());

            std::string searchQuery = historySearchBuf;
            std::transform(searchQuery.begin(), searchQuery.end(), searchQuery.begin(), ::tolower);

            for (int i = 0; i < (int)State.PlayerHistory.size(); ++i)
            {
                auto& p = State.PlayerHistory[i];
                auto itf = State.platformFilters.find(p.Platform);
                bool visible = (itf != State.platformFilters.end()) ? itf->second : true;
                if (!visible) continue;

                if (!searchQuery.empty()) {
                    std::string lnick = p.Nick;
                    std::transform(lnick.begin(), lnick.end(), lnick.begin(), ::tolower);
                    std::string lfc = p.FriendCode;
                    std::transform(lfc.begin(), lfc.end(), lfc.begin(), ::tolower);
                    std::string lpuid = p.Puid;
                    std::transform(lpuid.begin(), lpuid.end(), lpuid.begin(), ::tolower);

                    if (lnick.find(searchQuery) == std::string::npos &&
                        lfc.find(searchQuery) == std::string::npos &&
                        lpuid.find(searchQuery) == std::string::npos)
                        continue;
                }
                std::string decorated = p.Nick;

                if (p.NameCheck) decorated = "[!] " + decorated;
                bool inWL = std::find(State.WhitelistFriendCodes.begin(), State.WhitelistFriendCodes.end(), p.FriendCode) != State.WhitelistFriendCodes.end();
                bool inBL = std::find(State.BlacklistFriendCodes.begin(), State.BlacklistFriendCodes.end(), p.FriendCode) != State.BlacklistFriendCodes.end();
                if (inWL) decorated = "[+] " + decorated;
                if (inBL) decorated = "[-] " + decorated;

                decoratedStorage.push_back(std::move(decorated));
                names.push_back(decoratedStorage.back().c_str());
                filteredIndices.push_back(i);
            }

            if (names.empty())
            {
                selectedIndex = -1;
            }
            else
            {
                if (selectedIndex >= (int)names.size()) selectedIndex = (int)names.size() - 1;

                ImGui::PushItemWidth(200);
                if (ImGui::ListBox("##PlayerList", &selectedIndex, names.data(), (int)names.size(), 10))
                {
                    if (selectedIndex < 0 || selectedIndex >= (int)filteredIndices.size()) selectedIndex = -1;
                }
                ImGui::PopItemWidth();

                if (selectedIndex >= 0)
                {
                    int realIndex = filteredIndices[selectedIndex];
                    auto& p = State.PlayerHistory[realIndex];

                    ImGui::SameLine();
                    ImGui::BeginGroup();

                    ImGui::Text("使用Mod客户端: %s", p.IsModded ? "是" : "否");
                    if (p.IsModded && !p.ModClient.empty()) ImGui::Text("客户端名称: %s", p.ModClient.c_str());
                    ImGui::NewLine();
                    ImGui::Text("好友代码: %s", p.FriendCode.c_str());
                    ImGui::Text("PUID: %s", p.Puid.c_str());
                    ImGui::Text("等级: %d", p.Level);
                    ImGui::Text("平台: %s", p.Platform.c_str());
                    ImGui::Text("名称检查器: %s", p.NameCheck ? "有" : "无");
                    ImGui::NewLine();

                    if (AnimatedButton("复制信息"))
                    {
                        std::string infoText = p.Nick + "的信息:\n" +
                            "平台: " + p.Platform + "\n" +
                            "等级: " + std::format("{}", p.Level) + "\n" +
                            "PUID: " + p.Puid + "\n" +
                            "好友代码: " + p.FriendCode + "\n"
                            "使用Mod客户端: " + (p.IsModded ? "是" : "否") + "\n" +
                            (p.IsModded && !p.ModClient.empty() ? ("客户端名称: " + p.ModClient + "\n") : "") +
                            "名称检查器: " + (p.NameCheck ? "是" : "否");
                        ClipboardHelper_PutClipboardString(convert_to_string(infoText), NULL);
                    }

                    if (AnimatedButton("清理玩家"))
                    {
                        State.RemovedPlayers.insert(p.FriendCode);
                        State.PlayerHistory.erase(State.PlayerHistory.begin() + realIndex);
                        State.Save();
                        selectedIndex = -1;
                    }

                    ImGui::EndGroup();
                    ImGui::Spacing();

                    bool inWL = std::find(State.WhitelistFriendCodes.begin(), State.WhitelistFriendCodes.end(), p.FriendCode) != State.WhitelistFriendCodes.end();
                    std::string wLabel = inWL ? "从白名单移除" : "添加进白名单";

                    if (AnimatedButton(wLabel.c_str()))
                    {
                        if (inWL)
                            RemoveFromWhitelist(p.FriendCode);
                        else
                        {
                            AddToWhitelist(p.FriendCode);
                            RemoveFromBlacklist(p.FriendCode);
                        }
                        State.Save();
                        p;
                    }

                    ImGui::SameLine();

                    bool inBL = std::find(State.BlacklistFriendCodes.begin(), State.BlacklistFriendCodes.end(), p.FriendCode) != State.BlacklistFriendCodes.end();
                    std::string bLabel = inBL ? "从黑名单移除" : "添加进黑名单";

                    if (AnimatedButton(bLabel.c_str()))
                    {
                        if (inBL)
                            RemoveFromBlacklist(p.FriendCode);
                        else
                        {
                            AddToBlacklist(p.FriendCode);
                            RemoveFromWhitelist(p.FriendCode);
                        }
                        State.Save();
                    }

                    ImGui::SameLine();

                    std::string lowName = p.Nick;
                    std::transform(lowName.begin(), lowName.end(), lowName.begin(), ::tolower);
                    std::string ncLabel = p.NameCheck ? "从名称检查器中移除" : "添加进名称检查器";

                    if (AnimatedButton(ncLabel.c_str()))
                    {
                        if (p.NameCheck)
                        {
                            State.LockedNames.erase(std::remove(State.LockedNames.begin(), State.LockedNames.end(), lowName), State.LockedNames.end());
                            p.NameCheck = false;
                        }
                        else
                        {
                            State.LockedNames.push_back(lowName);
                            p.NameCheck = true;
                        }
                        for (auto& rp : State.PlayerHistory) {
                            std::string lc = rp.Nick;
                            std::transform(lc.begin(), lc.end(), lc.begin(), ::tolower);
                            rp.NameCheck = (std::find(State.LockedNames.begin(), State.LockedNames.end(), lc) != State.LockedNames.end());
                        }
                        State.Save();
                    }
                }
            }

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);

            if (ImGui::Button("清除历史"))
            {
                for (auto& pp : State.PlayerHistory) State.RemovedPlayers.insert(pp.FriendCode);
                State.PlayerHistory.clear();
                selectedIndex = -1;
                State.Save();
            }
            ImGui::SameLine(0, 20);
            if (ImGui::Button("更新玩家历史"))
            {
                bool changed = false;
                for (auto pctrl : GetAllPlayerControl())
                {
                    if (!pctrl || pctrl == *Game::pLocalPlayer) continue;
                    auto data = GetPlayerData(pctrl);
                    if (!data || data->fields.Disconnected) continue;

                    std::string fc = convert_from_string(data->fields.FriendCode);
                    std::string name = strToLower(RemoveHtmlTags(convert_from_string(GetPlayerOutfit(data)->fields.PlayerName)));
                    std::string puid = convert_from_string(data->fields.Puid);
                    int level = data->fields.PlayerLevel + 1;

                    if (fc.empty() || name.empty() || level <= 0) continue;
                    if (State.RemovedPlayers.count(fc)) State.RemovedPlayers.erase(fc);

                    bool exists = false;
                    for (auto& rp : State.PlayerHistory) if (rp.FriendCode == fc) { exists = true; break; }
                    if (exists) continue;

                    std::string platform = "未知";
                    auto client = app::InnerNetClient_GetClientFromCharacter((InnerNetClient*)(*Game::pAmongUsClient), pctrl, NULL);
                    if (client != nullptr && client->fields.PlatformData != nullptr && pctrl->fields._.OwnerId == client->fields.Id) {
                        switch (client->fields.PlatformData->fields.Platform) {
                        case Platforms__Enum::StandaloneEpicPC:
                            platform = "Epic Games (PC)";
                            break;
                        case Platforms__Enum::StandaloneSteamPC:
                            platform = "Steam (PC)";
                            break;
                        case Platforms__Enum::StandaloneMac:
                            platform = "Mac";
                            break;
                        case Platforms__Enum::StandaloneWin10:
                            platform = "Microsoft Store (PC)";
                            break;
                        case Platforms__Enum::StandaloneItch:
                            platform = "itch.io (PC)";
                            break;
                        case Platforms__Enum::IPhone:
                            platform = "iOS/iPadOS (Mobile)";
                            break;
                        case Platforms__Enum::Android:
                            platform = "Android (Mobile)";
                            break;
                        case Platforms__Enum::Switch:
                            platform = "Nintendo Switch (Console)";
                            break;
                        case Platforms__Enum::Xbox:
                            platform = "Xbox (Console)";
                            break;
                        case Platforms__Enum::Playstation:
                            platform = "Playstation (Console)";
                            break;
                        default:
                            platform = "未知";
                            break;
                        }
                    }

                    std::string lcname = name;
                    std::transform(lcname.begin(), lcname.end(), lcname.begin(), ::tolower);
                    bool nameCheck = (std::find(State.LockedNames.begin(), State.LockedNames.end(), lcname) != State.LockedNames.end());

                    bool isCheater = false;
                    std::string cheatName = "";
                    int pid = data->fields.PlayerId;
                    auto modIt = State.modUsers.find(pid);
                    if (modIt != State.modUsers.end()) {
                        cheatName = RemoveHtmlTags(modIt->second);
                        isCheater = true;
                    }

                    if (State.PlayerHistory.size() >= 100)
                        State.PlayerHistory.pop_front();

                    State.PlayerHistory.push_back({ name, fc, puid, level, platform, nameCheck, isCheater, cheatName });
                    changed = true;
                }
                if (changed) State.Save();
            }

            ImGui::Dummy(ImVec2(5, 5)* State.dpiScale);

            if (ImGui::CollapsingHeader("平台过滤器"))
            {
                ImGui::Columns(2, NULL, false);

                for (size_t i = 0; i < PLATFORM_FILTERS.size(); i++)
                {
                    ToggleButton(PLATFORM_FILTERS[i].c_str(), &State.platformFilters[PLATFORM_FILTERS[i]]);

                    if (i == (PLATFORM_FILTERS.size() + 1) / 2 - 1)
                        ImGui::NextColumn();
                }

                ImGui::Columns(1);
            }
        }

        if (openOptions) {
            if ((IsInGame() || IsInLobby()) && GameOptions().HasOptions()) {
                GameOptions options;
                /*std::string hostText = std::format("Host: {}", RemoveHtmlTags(GetHostUsername()));
                ImGui::Text(const_cast<char*>(hostText.c_str()));*/

                if (options.GetGameMode() == GameModes__Enum::Normal)
                {
                    auto allPlayers = GetAllPlayerControl();
                    RoleRates roleRates = RoleRates(options, (int)allPlayers.size());
                    // this should be all the major ones. if people want more they're simple enough to add.
                    ImGui::Text("任务可视: %s", (options.GetBool(app::BoolOptionNames__Enum::VisualTasks) ? "开" : "关"));
                    switch (options.GetInt(app::Int32OptionNames__Enum::TaskBarMode)) {
                    case 0:
                        ImGui::Text("任务进度更新: 实时");
                        break;
                    case 1:
                        ImGui::Text("任务进度更新: 会议时");
                        break;
                    case 2:
                        ImGui::Text("任务进度更新: 从不");
                        break;
                    default:
                        ImGui::Text("任务进度更新: 其他");
                        break;
                    }
                    ImGui::Text("驱除确认: %s", (options.GetBool(app::BoolOptionNames__Enum::ConfirmImpostor) ? "开" : "关"));
                    switch (options.GetInt(app::Int32OptionNames__Enum::KillDistance)) {
                    case 0:
                        ImGui::Text("击杀距离: 近");
                        break;
                    case 1:
                        ImGui::Text("击杀距离: 中");
                        break;
                    case 2:
                        ImGui::Text("击杀距离: 远");
                        break;
                    default:
                        ImGui::Text("击杀距离: 其他");
                        break;
                    }

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("最多工程师: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Engineer));
                    ImGui::Text("工程师几率: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Engineer));
                    ImGui::Text("工程师管道冷却: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::EngineerCooldown, 1.0F));
                    ImGui::Text("工程师管道时间: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::EngineerInVentMaxTime, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("最多科学家: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Scientist));
                    ImGui::Text("科学家几率: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Scientist));
                    ImGui::Text("科学家查看冷却: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ScientistCooldown, 1.0F));
                    ImGui::Text("科学家电池持续时间: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ScientistBatteryCharge, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("最多守护天使: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::GuardianAngel));
                    ImGui::Text("守护天使几率: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::GuardianAngel));
                    ImGui::Text("守护天使保护冷却时间: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::GuardianAngelCooldown, 1.0F));
                    ImGui::Text("守护天使保护时长: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ProtectionDurationSeconds, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("最多大嗓门: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Noisemaker));
                    ImGui::Text("大嗓门几率: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Noisemaker));
                    ImGui::Text("大嗓门警告时间: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::NoisemakerAlertDuration, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("最多侦查者: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Tracker));
                    ImGui::Text("侦查者几率: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Tracker));
                    ImGui::Text("侦查冷却: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::TrackerDuration, 1.0F));
                    ImGui::Text("侦查时间: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::TrackerCooldown, 1.0F));
                    ImGui::Text("侦查延迟: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::TrackerDelay, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("最多侦探: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Detective));
                    ImGui::Text("侦探几率: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Detective));
                    ImGui::Text("侦探嫌疑人限制: %.2f", options.GetFloat(app::FloatOptionNames__Enum::DetectiveSuspectLimit, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3)* State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3)* State.dpiScale);

                    ImGui::Text("最多变形者: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Shapeshifter));
                    ImGui::Text("变形者几率: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Shapeshifter));
                    ImGui::Text("变形者变形冷却: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ShapeshifterCooldown, 1.0F));
                    ImGui::Text("变形者变形时长: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ShapeshifterDuration, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("最多幻象师: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Phantom));
                    ImGui::Text("幻象师几率: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Phantom));
                    ImGui::Text("幻象师隐形冷却: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::PhantomCooldown, 1.0F));
                    ImGui::Text("幻象师隐形时间: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::PhantomDuration, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("最多毒蛇: %d", roleRates.GetRoleCount(app::RoleTypes__Enum::Viper));
                    ImGui::Text("毒蛇几率: %d%", options.GetRoleOptions().GetChancePerGame(RoleTypes__Enum::Viper));
                    ImGui::Text("毒蛇溶解时间: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::ViperDissolveTime, 1.0F));
                }
                else if (options.GetGameMode() == GameModes__Enum::HideNSeek) {

                    int ImpostorId = options.GetInt(app::Int32OptionNames__Enum::ImpostorPlayerID);
                    if (ImpostorId < 0) {
                        ImGui::Text("Impostor: Round-robin");
                    }
                    else {
                        std::string ImpostorName = std::format("指定伪装者: {}", convert_from_string(NetworkedPlayerInfo_get_PlayerName(GetPlayerDataById(ImpostorId), nullptr)));
                        ImGui::Text(const_cast<char*>(ImpostorName.c_str()));
                    }
                    ImGui::Text("手电筒模式: %s", (options.GetBool(app::BoolOptionNames__Enum::UseFlashlight) ? "On" : "Off"));
                    ImGui::Text("显示名称: %s", (options.GetBool(app::BoolOptionNames__Enum::ShowCrewmateNames) ? "On" : "Off"));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("最多通风管道使用: %d", options.GetInt(app::Int32OptionNames__Enum::CrewmateVentUses));
                    ImGui::Text("通风管道时间: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::CrewmateTimeInVent, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3) * State.dpiScale);

                    ImGui::Text("躲藏时间: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::EscapeTime, 1.0F));
                    ImGui::Text("终局躲藏时间: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::FinalEscapeTime, 1.0F));
                    ImGui::Text("终局伪装者速度: %.2f s", options.GetFloat(app::FloatOptionNames__Enum::SeekerFinalSpeed, 1.0F));

                    ImGui::Dummy(ImVec2(3, 3)* State.dpiScale);
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(3, 3)* State.dpiScale);

                    ImGui::Text("船员手电筒大小: % .2fx", options.GetFloat(app::FloatOptionNames__Enum::CrewmateFlashlightSize, 1.0F));
                    ImGui::Text("伪装者手电筒大小: % .2fx", options.GetFloat(app::FloatOptionNames__Enum::ImpostorFlashlightSize, 1.0F));
                    ImGui::Text("终局船员寻找器: %s", (options.GetBool(app::BoolOptionNames__Enum::SeekerFinalMap) ? "On" : "Off"));
                    ImGui::Text("终局位置提示: %s", (options.GetBool(app::BoolOptionNames__Enum::SeekerPings) ? "On" : "Off"));
                    ImGui::Text("提示间隔: % .2f s", options.GetFloat(app::FloatOptionNames__Enum::MaxPingTime, 1.0F));
                }
            }
            else CloseOtherGroups(Groups::General);
        }
        ImGui::EndChild();
    }
}
