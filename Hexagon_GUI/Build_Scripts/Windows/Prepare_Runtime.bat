@ECHO OFF
COLOR 0A

SET QT=Qt_6_5_2_MinGW_64_bit
SET MODE=%1
IF %1.==. SET MODE=Debug
SET SOURCE_LOCATION=D:\Documents\Source_Code

REM Prepare Folders
MKDIR "%SOURCE_LOCATION%\Hexagon\build-Hexagon_GUI-Desktop_%QT%-%MODE%\Plugins" >NUL 2>&1

REM Delete Old Plugins
ERASE "%SOURCE_LOCATION%\Hexagon\build-Hexagon_GUI-Desktop_%QT%-%MODE%\Plugins\Hexagon.dll" >NUL 2>&1

REM Install New Plugins
COPY "%SOURCE_LOCATION%\Hexagon\build-Hexagon-Desktop_%QT%-%MODE%\libHexagon.dll" "%SOURCE_LOCATION%\Hexagon\build-Hexagon_GUI-Desktop_%QT%-%MODE%\Plugins\Hexagon.dll"
