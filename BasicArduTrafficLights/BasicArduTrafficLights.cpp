#include "BasicArduTrafficLights.h"

BasicArduTrafficLights::BasicArduTrafficLights()
{
    timer_ = new sf::Clock();

    comboValues[0] = "Green";
    comboValues[1] = "Yellow";
    comboValues[2] = "Red";
}

BasicArduTrafficLights::~BasicArduTrafficLights()
{
    delete timer_;
}

void BasicArduTrafficLights::RenderAndHandleEvents(ClientUDPThread &InUDPSender)
{
    ImGui::Begin("Traffic lights");

    ImGui::Checkbox("Manual control", &manualControlFlag_);

    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !manualControlFlag_);
    ImGui::Combo("Traffic light left", &manualTrafficLeft, comboValues, IM_ARRAYSIZE(comboValues));
    ImGui::Combo("Traffic light right", &manualTrafficRight, comboValues, IM_ARRAYSIZE(comboValues));
    ImGui::PopItemFlag();


    if ( !manualControlFlag_ )
    {
        switch (trafLight1State)
        {
            case 0:
            {
                manualTrafficLeft = 0;
                manualTrafficRight = 2;

                if (timer_->getElapsedTime().asSeconds() > trafLightDelay1)
                {
                    trafLightTmp = 0;
                    trafLight1State = 1;
                    timer_->restart();
                }
                break;
            }
            case 1:
            {
                manualTrafficLeft = 1;
                manualTrafficRight = 1;

                if (timer_->getElapsedTime().asSeconds() > trafLightDelay2)
                {
                    if (trafLightTmp == 0) trafLight1State = 2;
                    else if (trafLightTmp == 2) trafLight1State = 0;
                    timer_->restart();
                }
                break;
            }
            case 2:
            {
                manualTrafficLeft = 2;
                manualTrafficRight = 0;

                if (timer_->getElapsedTime().asSeconds() > trafLightDelay1)
                {
                    trafLightTmp = 2;
                    trafLight1State = 1;
                    timer_->restart();
                }
                break;
            }
        }
    }
    InUDPSender.SendMessage( std::to_string(manualTrafficLeft) + std::to_string(manualTrafficRight) );

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 center = ImGui::GetCursorScreenPos();
    auto size = ImGui::GetWindowSize();

    float radius = 50.0f;

    center.x += 100;
    center.y += (radius + 10);

//    ImU32 color = IM_COL32(255, 0, 0, 255);
//
//    draw_list->AddCircleFilled(center, radius, color);
//    draw_list->AddCircleFilled({center.x + 130, center.y}, radius, color);

    switch (manualTrafficLeft)
    {
        case 0: draw_list->AddCircleFilled(center, radius, green); break;
        case 1: draw_list->AddCircleFilled(center, radius, yellow); break;
        case 2: draw_list->AddCircleFilled(center, radius, red); break;
    }
    switch (manualTrafficRight)
    {
        case 0: draw_list->AddCircleFilled({center.x + 130, center.y}, radius, green); break;
        case 1: draw_list->AddCircleFilled({center.x + 130, center.y}, radius, yellow); break;
        case 2: draw_list->AddCircleFilled({center.x + 130, center.y}, radius, red); break;
    }

    ImGui::End();
}