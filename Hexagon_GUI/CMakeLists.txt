cmake_minimum_required(VERSION 3.14)

project(Hexagon_GUI LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wdeprecated -Werror")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets REQUIRED)

set(SOURCES
    main.cpp
    Main_Window.cpp
    Settings_File.cpp
    String_Manipulator.cpp
    Error_Messages.cpp
    File_Dialog_Manager.cpp
    Conflicts_Window.cpp
    ../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.cpp
    ../../C_Common_Code/Qt/Qt_App_Theme_Fixes/Fusion_Fixes.cpp
)

set(HEADERS
    Common_Strings.h
    Main_Window.h
    Version.h
    Settings_File.h
    Settings.h
    String_Manipulator.h
    Error_Messages.h
    File_Dialog_Manager.h
    File_Types.h
    Conflicts_Window.h
    ../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.h
    ../../C_Common_Code/Qt/Qt_App_Theme_Fixes/Fusion_Fixes.h
)

set(FORMS
    Main_Window.ui
    Conflicts_Window.ui
)

set(RC_FILE Hexagon_GUI.rc)

# Remove WIN32 if you want the console window
add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS} ${FORMS} ${RC_FILE})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
