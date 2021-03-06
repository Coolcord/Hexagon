#-------------------------------------------------
#
# Project created by QtCreator 2018-06-06T20:42:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Hexagon_GUI
TEMPLATE = app
CONFIG += c++11 pendantic Weverything

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        Main_Window.cpp \
    Settings_File.cpp \
    String_Manipulator.cpp \
    Error_Messages.cpp \
    File_Dialog_Manager.cpp \
    Conflicts_Window.cpp \
    ../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.cpp

HEADERS += \
    Common_Strings.h \
    Main_Window.h \
    Version.h \
    Settings_File.h \
    Settings.h \
    String_Manipulator.h \
    Error_Messages.h \
    File_Dialog_Manager.h \
    File_Types.h \
    Conflicts_Window.h \
    ../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.h

FORMS += \
        Main_Window.ui \
    Conflicts_Window.ui

RC_FILE = Hexagon_GUI.rc

OTHER_FILES +=
