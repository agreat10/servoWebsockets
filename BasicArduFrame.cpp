#include "include/BasicArduFrame.h"
#include <iostream>

// конструктор
BasicArduFrame::BasicArduFrame() {

}
// деструктор
BasicArduFrame::~BasicArduFrame() {

}

// отрисовать и обработать события элементов управления
void BasicArduFrame::RenderAndHandleEvents(ClientUDPThread &InUDPSender) {
    ImGui::Begin("Basic");
    if(mIsLedOn){
        if (ImGui::Button(cLEDOFF)) {
            mIsLedOn = false;
            InUDPSender.SendMessage(sLedOff);
        }
    } else {
        if (ImGui::Button(cLEDON)) {
            mIsLedOn = true;
            InUDPSender.SendMessage(sLedOn);
        }     
    }
    if (ImGui::Button(cWatchGYRO)) {
        InUDPSender.SendMessage(sWatchGYRO);
    }
    if (ImGui::Button(cStopWatchGYRO)) {
        InUDPSender.SendMessage(sStopWatchGYRO);
    }     
    if(InUDPSender.HasMessages()) {
        InUDPSender.PopRecievedMessage(mTextMultiLine, sizeof(mTextMultiLine));    
    }
    if(ImGui::SliderInt("ESC1 Value", &ESC1Value, 0, 100)) {
        InUDPSender.SendMessage("ESC1=" + std::to_string(ESC1Value));
    }
    if(ImGui::SliderInt("ESC2 Value", &ESC2Value, 0, 100)) {
        InUDPSender.SendMessage("ESC2=" + std::to_string(ESC2Value));
    }
    if(ImGui::SliderInt("ESC3 Value", &ESC3Value, 0, 100)) {
        InUDPSender.SendMessage("ESC3=" + std::to_string(ESC3Value));
    }
    if(ImGui::SliderInt("ESC4 Value", &ESC4Value, 0, 100)) {
        InUDPSender.SendMessage("ESC4=" + std::to_string(ESC4Value));
    }
    ImGui::LabelText("##Arduino Packets", mTextMultiLine);
    ImGui::End();
}