#!/bin/bash

MODE="Debug"
SOURCE_LOCATION="/home/cord/Documents/Source_Code"

echo "Installing plugins for Hexagon GUI..."
mkdir -p "$SOURCE_LOCATION"/Hexagon/build-Hexagon_GUI-Desktop-"$MODE"/Plugins
cp -rf "$SOURCE_LOCATION"/Hexagon/build-Hexagon-Desktop-"$MODE"/libHexagon.so "$SOURCE_LOCATION"/Hexagon/build-Hexagon_GUI-Desktop-"$MODE"/Plugins/Hexagon.so

echo "Runtime Environment Prepared!"
exit 0

