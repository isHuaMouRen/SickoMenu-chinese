#include "pch-il2cpp.h"
#include "sabotage_tab.h"
#include "utility.h"
#include "gui-helpers.hpp"
#include "_rpc.h"
#include "game.h"
#include "state.hpp"
#include <random>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>

namespace SabotageTab {
    // Begone, garbage code.

    void Render() {
        if (IsInGame()) {
            ImGui::SameLine(100 * State.dpiScale);
            ImGui::BeginChild("###Sabotage", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
            if (IsHost() && ToggleButton("禁用破坏", &State.DisableSabotages)) {
                ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
                ImGui::Separator();
                ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
            }
            if (AnimatedButton("修复破坏")) {
                RepairSabotage(*Game::pLocalPlayer);
            }

            if (ToggleButton("自动修复破坏", &State.AutoRepairSabotage)) {
                State.Save();
            }

            ImGui::NewLine();
            if (State.DisableSabotages)
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "破坏已被禁用.");
            //i skidded some code from https://github.com/scp222thj/MalumMenu/

            if (AnimatedButton("破坏所有")) {
                if (State.mapType != Settings::MapType::Fungle) {
                    for (uint32_t i = 0; i < 5; i++)
                        State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Electrical, i));
                }

                if (State.mapType == Settings::MapType::Ship || State.mapType == Settings::MapType::Hq || State.mapType == Settings::MapType::Fungle)
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Reactor, 128));
                else if (State.mapType == Settings::MapType::Pb)
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Laboratory, 128));
                else if (State.mapType == Settings::MapType::Airship)
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::HeliSabotage, 128));

                if (State.mapType == Settings::MapType::Ship || State.mapType == Settings::MapType::Hq)
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::LifeSupp, 128));

                if (State.mapType == Settings::MapType::Fungle)
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::MushroomMixupSabotage, 1));

                State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Comms, 128));
            }

            if (AnimatedButton("随机破坏")) {
                switch (State.mapType) {
                case Settings::MapType::Pb:
                {
                    int randIndex = randi(1, 3);
                    switch (randIndex) {
                    case 1:
                    {
                        for (uint32_t i = 0; i < 5; i++)
                            State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Electrical, i));
                    } break;
                    case 2: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Laboratory, 128)); break;
                    case 3: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Comms, 128)); break;
                    }
                }
                break;
                case Settings::MapType::Airship:
                {
                    int randIndex = randi(1, 3);
                    switch (randIndex) {
                    case 1:
                    {
                        for (uint32_t i = 0; i < 5; i++)
                            State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Electrical, i));
                    } break;
                    case 2: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::HeliSabotage, 128)); break;
                    case 3: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Comms, 128)); break;
                    }
                }
                break;
                case Settings::MapType::Fungle:
                {
                    int randIndex = randi(1, 3);
                    switch (randIndex) {
                    case 1: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::MushroomMixupSabotage, 1)); break;
                    case 2: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Reactor, 128)); break;
                    case 3: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Comms, 128)); break;
                    }
                }
                break;
                default:
                {
                    int randIndex = randi(1, 4);
                    switch (randIndex) {
                    case 1:
                    {
                        for (uint32_t i = 0; i < 5; i++)
                            State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Electrical, i));
                    } break;
                    case 2: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Reactor, 128)); break;
                    case 3: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::LifeSupp, 128)); break;
                    case 4: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Comms, 128)); break;
                    }
                }
                break;
                }
            }

            if (State.mapType != Settings::MapType::Fungle && AnimatedButton("破坏灯光")) {
                for (uint32_t i = 0; i < 5; i++)
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Electrical, i));
            }
            if (State.mapType == Settings::MapType::Ship || State.mapType == Settings::MapType::Hq || State.mapType == Settings::MapType::Fungle) {
                if (AnimatedButton("破坏反应堆")) {
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Reactor, 128));
                }
            }
            else if (State.mapType == Settings::MapType::Pb) {
                if (AnimatedButton("破坏地震稳定器")) {
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Laboratory, 128));
                }
            }
            else if (State.mapType == Settings::MapType::Airship) {
                if (AnimatedButton("坠机风险")) {
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::HeliSabotage, 128));
                }
            }
            if (State.mapType == Settings::MapType::Ship || State.mapType == Settings::MapType::Hq) {
                if (AnimatedButton("破坏氧气")) {
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::LifeSupp, 128));
                }
            }
            if (State.mapType == Settings::MapType::Fungle) {
                if (AnimatedButton("激活蘑菇混合物")) {
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::MushroomMixupSabotage, 1));
                }
            }
            if (AnimatedButton("破坏通讯")) {
                State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Comms, 128));
            }

            ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

            if (State.mapType != Settings::MapType::Fungle) {
                if (ToggleButton("禁用灯光", &State.DisableLights)) {
                    if (auto switchSystem = (SwitchSystem*)il2cpp::Dictionary((*Game::pShipStatus)->fields.Systems)[SystemTypes__Enum::Electrical]) {
                        auto actualSwitches = switchSystem->fields.ActualSwitches;
                        auto expectedSwitches = switchSystem->fields.ExpectedSwitches;

                        auto switchMask = 1 << ((State.DisableLights ? 0 : 5) & 0x1F);

                        if ((actualSwitches & switchMask) != ((State.DisableLights ? ~expectedSwitches : expectedSwitches) & switchMask)) {
                            State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Electrical, 5));
                        }
                    }
                }
            }
            ImGui::SameLine();
            if (ToggleButton("破坏灯光 (混乱切换开关)", &State.DisableLightSwitches)) State.Save();

            if (ToggleButton("禁用修复通信", &State.DisableComms)) State.Save();

            if (ToggleButton("混乱反应堆", &State.DisableReactor)) State.Save();

            if ((State.mapType == Settings::MapType::Ship || State.mapType == Settings::MapType::Hq) && ToggleButton("混乱破坏氧气", &State.DisableOxygen))
                State.Save();

            if (State.mapType == Settings::MapType::Fungle && ToggleButton("无限蘑菇混合物", &State.InfiniteMushroomMixup))
                State.Save();

            ImGui::EndChild();
        }
    }
}