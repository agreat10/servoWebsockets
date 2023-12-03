#include "./BasicArduLCD.h"

void BasicArduLCD::RenderAndHandleEvents(ClientUDPThread &InUDPSender)
{
    ImGui::Begin("LCD display");

    ImGui::InputText("First row", firstRow, 16);
    ImGui::InputText("Second row", secondRow, 16);
    ImGui::Checkbox("BackLight", &backLight);
    if ( ImGui::Button("Send") )
    {
        InUDPSender.SendMessage(std::string(firstRow) + "\n" + std::string(secondRow));
    }
    ImGui::End();
}