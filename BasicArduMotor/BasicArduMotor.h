#pragma once

#include <include/ClientUDPThread.h>

#include <imgui/imgui.h>

class BasicArduMotor
{
public:
    BasicArduMotor() = default;
    ~BasicArduMotor() = default;

    void display(ClientUDPThread& udpThread)
    {
        ImGui::Begin("Example1");
        ImGui::SliderInt("Motor speed", &slider_value, 0, 255);
        ImGui::Checkbox("Invert direction", &invert_direction);
        if ( invert_direction ) udpThread.SendMessage( std::string(std::to_string(-slider_value)));
        else udpThread.SendMessage( std::string(std::to_string(slider_value)));
        ImGui::End();
    }
private:
    int slider_value = 0;
    bool invert_direction = false;
};