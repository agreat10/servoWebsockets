#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Network.hpp>

#include <string>
#include <iostream>
#include <vector>
#include <include/ClientUDPThread.h>

class BasicArduLCD
{
public:
    BasicArduLCD() = default;
    ~BasicArduLCD() = default;
    void RenderAndHandleEvents(ClientUDPThread &InUDPSender);

private:
    char firstRow[16] = "";
    char secondRow[16] = "";
    bool backLight = false;
};