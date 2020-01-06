@ECHO OFF
COLOR 0A

SET QT=Qt_5_12_6_MinGW_32_bit
SET MODE=%1
IF %1.==. SET MODE=Debug
SET SOURCE_LOCATION=F:\Documents\Source_Code

REM Prepare Folders
MKDIR "%SOURCE_LOCATION%\Hexagon\build-Hexagon_GUI-Desktop_%QT%-%MODE%\%MODE%\Plugins" >NUL 2>&1

REM Delete Old Plugins
ERASE "%SOURCE_LOCATION%\Hexagon\build-Hexagon_GUI-Desktop_%QT%-%MODE%\%MODE%\Plugins\Hexagon.dll" >NUL 2>&1

REM Install New Plugins
COPY "%SOURCE_LOCATION%\Hexagon\build-Hexagon-Desktop_%QT%-%MODE%\%MODE%\Hexagon.dll" "%SOURCE_LOCATION%\Hexagon\build-Hexagon_GUI-Desktop_%QT%-%MODE%\%MODE%\Plugins\Hexagon.dll"
