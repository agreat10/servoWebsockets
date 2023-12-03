cmake_minimum_required(VERSION 3.6)

project(ArduinoUDPClient VERSION 1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories( ${PROJECT_SOURCE_DIR}/ )
include_directories( ${PROJECT_SOURCE_DIR}/include )
include_directories( ${PROJECT_SOURCE_DIR}/imgui )

find_package(SFML COMPONENTS system graphics audio network REQUIRED)

set(IMGUI
        imgui/imconfig.h
        imgui/imgui.cpp
        imgui/imgui.h
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_internal.h
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        imgui/imconfig-SFML.h
        imgui/imgui-SFML.cpp
        imgui/imgui-SFML.h
        imgui/imgui-SFML_export.h
)

set(HEADERS
        include/BasicArduFrame.h
        include/ClientUDPThread.h
)

set(SOURCES
        BasicArduFrame.cpp
        ClientUDPThread.cpp
        main.cpp
)

#set(BAMOTOR
#        BasicArduMotor/BasicArduMotor.h
#)
#
#set(BCC
#        BasicCarController/BasicCarController.h
#)

set(BATL
        BasicArduTrafficLights/BasicArduTrafficLights.h
        BasicArduTrafficLights/BasicArduTrafficLights.cpp
)

set(BALCD
        BasicArduLCD/BasicArduLCD.h
        BasicArduLCD/BasicArduLCD.cpp
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${IMGUI} ${BATL} ${BALCD} ${BAMOTOR} ${BCC} ClientWebSocketThread.h)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE sfml-window sfml-network sfml-graphics sfml-system websockets opengl32)
endif()

if(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE sfml-window sfml-network sfml-graphics sfml-system GL websockets)
endif()
