cmake_minimum_required(VERSION 3.16)
project(standUP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(standUP
    main.cpp
    AppSettings.h
    AudioPlayer.h
    AudioPlayer.cpp
    SystemAlert.h
    SystemAlert.cpp
    MainScreen.h
    MainScreen.cpp
    ConfigurationPanel.h
    ConfigurationPanel.cpp
    standUP.h
    standUP.cpp
)

target_link_libraries(standUP PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
)