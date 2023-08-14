cmake_minimum_required(VERSION 3.14)

project(Hexagon LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wdeprecated -Werror")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

set(SOURCES
    Hexagon.cpp
    Patch_Buffer.cpp
    Patch_Reader.cpp
    Patch_Writer.cpp
    Qt_Code_Reader.cpp
    Qt_Code_Writer.cpp
    Value_Manipulator.cpp
    File_Writer.cpp
    Patch_Comparer.cpp
    File_Comparer.cpp
)

set(HEADERS
    Hexagon.h
    Hexagon_Interface.h
    Patch_Buffer.h
    Patch_Strings.h
    Patch_Reader.h
    Patch_Writer.h
    Qt_Code_Reader.h
    Qt_Code_Writer.h
    Qt_Code_Strings.h
    Value_Manipulator.h
    File_Writer.h
    Patch_Comparer.h
    File_Comparer.h
    Hexagon_Error_Codes.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)
