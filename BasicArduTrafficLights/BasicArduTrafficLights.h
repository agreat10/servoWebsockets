#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "imgui-SFML.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Network.hpp>

#include <include/ClientUDPThread.h>

#include <string>
#include <iostream>
#include <vector>

class BasicArduTrafficLights
{
public:
    BasicArduTrafficLights();
    ~BasicArduTrafficLights();
    void RenderAndHandleEvents(ClientUDPThread &InUDPSender);

private:
    bool manualControlFlag_ = false;

    // 0 -> green
    // 1 -> yellow
    // 2 -> red
    size_t trafLightTmp = 0;
    size_t trafLight1State = 1;
    int trafLightDelay1 = 10;
    int trafLightDelay2 = 1;
    sf::Clock* timer_ = nullptr;

    const char* comboValues[3]{};
    int manualTrafficLeft = 1;
    int manualTrafficRight = 1;

    ImColor green{34,139,34};
    ImColor yellow{255,255,53};
    ImColor red{208,1,1};
};