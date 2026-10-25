cmake_minimum_required(VERSION 3.16)
project(standUP VERSION 0.10.0 LANGUAGES CXX)

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

target_compile_definitions(standUP PRIVATE
    APP_VERSION="${PROJECT_VERSION}"
)

include(GNUInstallDirs)
install(TARGETS standUP
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES standUP.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
)

# CPack configuration for generating .deb packages
set(CPACK_PACKAGE_NAME "standup")
set(CPACK_PACKAGE_VENDOR "Misty Mountain Group")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Periodic stand-up and break reminder")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "rwg")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
#set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6multimedia6-plugins")

include(CPack)
