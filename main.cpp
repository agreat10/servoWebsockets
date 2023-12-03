#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Network.hpp>

#include <ClientWebSocketThread.h>

#include <thread>

sf::Clock clock_;
bool activFlag = false;
bool flag = false;
int minAngle = 0;
int maxAngle = 180;
int sliderValue1 = minAngle;
ImGuiSliderFlags sliderFlag = ImGuiSliderFlags_None;
sf::Clock timer_;
int timerValue = 600;
int timerStep = 31;
bool timerFlag = false;
void display3()
{
    ImGui::Begin("Servo");
    ImGui::Checkbox("ON | OFF", &activFlag);
    if(activFlag)
    {
        if(ImGui::SliderInt("Serva", &sliderValue1, minAngle, maxAngle, "", sliderFlag))
        {
            flag = true;
        }
        else if(flag)
        {
            flag = false;
            SendMessage(std::to_string(sliderValue1));

        }
    }
    else
    {
        ImGui::Text("%s", std::to_string(sliderValue1).c_str());
        ImGui::SliderInt("Step", &timerStep, 0, 180);
        ImGui::SliderInt("Delay", &timerValue, 100, 1000);
        if(timer_.getElapsedTime().asMilliseconds() >= timerValue)
        {
            if(timerFlag)
            {
                sliderValue1 += timerStep;
            }
            else if(!timerFlag)
            {
                sliderValue1 -= timerStep;
            }
            if(sliderValue1 >= 180)
            {
                timerFlag = false;
            }
            if(sliderValue1 <= 0)
            {
                timerFlag = true;
            }
            SendMessage(std::to_string(sliderValue1));
            timer_.restart();
        }
    }
    ImGui::End();
}

int main()
{
    // Нужно для запуска потока с сервером
    memset(&info, 0, sizeof info);
    info.port = 41235;
    info.protocols = protocols;
    context = lws_create_context(&info);
    std::thread t1(run);
    t1.detach();

    sf::RenderWindow window(sf::VideoMode(800, 480), "Arduino UDP");
    window.setFramerateLimit(60);

    if(ImGui::SFML::Init(window))
    {
        sf::Clock deltaClock;
        while (window.isOpen())
        {
            sf::Event event{};
            while (window.pollEvent(event))
            {
                ImGui::SFML::ProcessEvent(window, event);
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
            }
            ImGui::SFML::Update(window, deltaClock.restart());

            // Вызов функции
            display3();

            window.clear();
            ImGui::SFML::Render(window);
            window.display();
        }
        ImGui::SFML::Shutdown();
    }

    running_ = false;
    lws_context_destroy(context);
    return 0;
}
