#ifndef BASIC_ARDU_FRAME_H
#define BASIC_ARDU_FRAME_H

#include <imgui/imgui.h>
#include "imgui-SFML.h"
#include "ClientUDPThread.h" // лучше было бы эти объекты увязать через делегаты, но код сильно усложнится

// объект реализующий фрейм базового управления ардуинкой
class BasicArduFrame {
  private:
    // UI - имена элементов управления
    const char* cLEDON = "LED ON";
    const char* cLEDOFF = "LED OFF";
    const char* cWatchGYRO = "WATCHGYRO";
    const char* cStopWatchGYRO = "STOPWATCHGYRO";
    // UDP текстовые команды
    std::string sLedOn = "LED ON"; // включить светодиод
    std::string sLedOff = "LED OFF"; // выключить светодиод
    std::string sWatchGYRO = "WATCHGYRO"; // включить наблюдение за гироскопом
    std::string sStopWatchGYRO = "STOPWATCH"; // отключить наблюдение
    bool mIsLedOn = false;
    char mTextMultiLine[1024] = "No data (just started)";
    // ESC - это контроллер управления безколлекторным электродвигателем. 
    // Управляется он так же как и сервопривод - через цифровой выход с временными интервалами импульсов
    // для удобства мы мысштабировали значения от 0 до 100. Но в ардуино плате это значения от 800 до 2300(зависит от контретной сервы)
    int ESC1Value = 0.0f; // значение ESC 1
    int ESC2Value = 0.0f; // значение ESC 2
    int ESC3Value = 0.0f; // значение ESC 3
    int ESC4Value = 0.0f; // значение ESC 4
  public:
    // конструктор
    BasicArduFrame();
    // деструктор
    ~BasicArduFrame();
      // отрисовать и обработать события элементов управления
      void RenderAndHandleEvents(ClientUDPThread &InUDPSender);
};

#endif