#!/bin/bash

# This must be set to the parent directory of Hexagon!
localSourceCodeLocation="/d/Documents/Source_Code"

# Make sure a version number was provided
if [ -z $1 ]; then
    echo "No version argument provided! The first argument must be the version!"
    echo ""
    echo "$0 <version>"
    echo ""
    echo "E.g.: $0 v0.4.0"
    exit 1
fi

# Remove old deployed files
rm -rf ./Deployed_Files >/dev/null 2>&1

# Get dependencies for Deploy Script
if [ ${MSYSTEM} == "MINGW64" ]; then
    dependencies="git rsync mingw-w64-x86_64-gcc mingw-w64-x86_64-ninja mingw-w64-x86_64-cmake mingw-w64-x86_64-qt6-base p7zip mingw-w64-x86_64-nsis"
    echo Checking dependencies for deploy...
    if ! pacman -Q $dependencies > /dev/null 2>&1; then
        echo Installing missing dependencies...
        pacman -Sy --needed --noconfirm $dependencies
    fi
fi

# Check if dependencies are installed
if [ ! -z $1 ] && ([ "$1" == "latest" ] || [ "$1" == "local" ]); then # TODO: After the next update, this code will be applied to stable!
    echo ""; echo [1/11] Preparing source code...
    command -v ninja >/dev/null 2>&1 || { echo >&2 "ninja must be installed before Hexagon can be compiled! Aborting!"; exit 1; }
fi
command -v git >/dev/null 2>&1 || { echo >&2 "git must be installed before Hexagon can be compiled! Aborting!"; exit 1; }
command -v gcc >/dev/null 2>&1 || { echo >&2 "gcc must be installed before Hexagon can be compiled! Aborting!"; exit 1; }
command -v nproc >/dev/null 2>&1 || { echo >&2 "nproc must be installed before Hexagon can be compiled! Aborting!"; exit 1; }
if [ ${MSYSTEM} == "MINGW64" ]; then
    command -v qtpaths6 >/dev/null 2>&1 || { echo >&2 "qtpaths6 must be installed before Hexagon can be compiled! Aborting!"; exit 1; }
    command -v ldd >/dev/null 2>&1 || { echo >&2 "ldd must be installed before Hexagon can be compiled! Aborting!"; exit 1; }
    if [ ! -f /mingw64/share/qt6/plugins/platforms/qwindows.dll ]; then
        echo "qwindows.dll could not be found! Aborting!"; exit 1;
    fi
    if [ ! -f /mingw64/share/qt6/plugins/styles/qwindowsvistastyle.dll ]; then
        echo "qwindowsvistastyle.dll could not be found! Aborting!"; exit 1;
    fi
	if [ ! -f /mingw64/share/qt6/plugins/tls/qschannelbackend.dll ]; then
        echo "qschannelbackend.dll could not be found! Aborting!"; exit 1;
    fi
fi
command -v 7z >/dev/null 2>&1 || { echo >&2 "p7zip must be installed before Hexagon can be deployed! Aborting!"; exit 1; }
command -v makensis >/dev/null 2>&1 || { echo >&2 "NSIS must be installed before Hexagon can be deployed! Aborting!"; exit 1; }
CPUcores=$(nproc)

# Add v to the version if it doesn't exist
version="$1"
if [[ "${original}" != v* ]]; then
    modified="v${original}"
fi
versionNumber="${version:1}"

# Split the parts into significant, major, minor, and patch versions
IFS=. read -r significant major minor <<< "$versionNumber"
IFS=- read -r minor patch <<< "$minor"
if [ -z $patch ]; then
    patch="0"
fi
if [[ $version == *-dev ]]; then
    patch="0"
fi
installerVersion="$significant.$major.$minor.$patch"

# Update the Hexagon version
sed -i "s/    const static QString VERSION_NUMBER = .*/    const static QString VERSION_NUMBER = \"$versionNumber\"\;/g" "$localSourceCodeLocation"/Hexagon/Hexagon_GUI/Version.h

echo [1/5] Preparing Hexagon source code...
mkdir -p ./Deployed_Files/source
cp -rf "$localSourceCodeLocation/C_Common_Code" ./Deployed_Files/source
cd ./Deployed_Files/source
mkdir -p Hexagon/Hexagon_GUI/Build_Scripts/Unix
copy_excluding_folder() {
    cp -f "$1"/* "$2" >/dev/null 2>&1
    find "$1" -mindepth 1 -maxdepth 1 -type d -exec basename {} \; > folders.txt
    sed -i "/$3/d" folders.txt
    while IFS= read -r folder; do
        cp -rf "$1"/"$folder" "$2"
    done < folders.txt
    rm folders.txt
}

# Copy Hexagon, but ignore "./Hexagon/Hexagon_GUI/Build_Scripts/Unix/Deployed_Files" in case this script is running there
copy_excluding_folder "$localSourceCodeLocation/Hexagon" "./Hexagon" "Hexagon_GUI"
copy_excluding_folder "$localSourceCodeLocation/Hexagon/Hexagon_GUI" "./Hexagon/Hexagon_GUI" "Build_Scripts"
copy_excluding_folder "$localSourceCodeLocation/Hexagon/Hexagon_GUI/Build_Scripts" "./Hexagon/Hexagon_GUI/Build_Scripts" "Unix"
copy_excluding_folder "$localSourceCodeLocation/Hexagon/Hexagon_GUI/Build_Scripts/Unix" "./Hexagon/Hexagon_GUI/Build_Scripts/Unix" "Deployed_Files"
cd Hexagon/

# Build Hexagon
echo ""; echo [2/5] Compiling Hexagon...
cd Hexagon/
rm -rf build >/dev/null 2>&1; mkdir -p build
cd build
cmake .. -G Ninja
ninja -j "$CPUcores" || exit 1
cd ../../

echo ""; echo [3/5] Compiling Hexagon GUI...
cd Hexagon_GUI/
rm -rf build >/dev/null 2>&1; mkdir -p build
cd build
cmake .. -G Ninja
ninja -j "$CPUcores" || exit 1
cd ../../

# Create the Runtime Environment
cd ../../
mkdir -p ./Hexagon/Plugins

# Install Plugins
echo ""; echo [4/5] Installing Plugins...
if [ -z ${MSYSTEM} ]; then # assume we're on GNU/Linux or Mac
    dllExt=".so"
    exeExt=""
else # assume we're on a Unix environment running in a Windows OS
    dllExt=".dll"
    exeExt=".exe"
fi

cp source/Hexagon/Hexagon_GUI/build/Hexagon_GUI"$exeExt" ./Hexagon/Hexagon"$exeExt"
chmod +x ./Hexagon/Hexagon"$exeExt"
cp source/Hexagon/Hexagon/build/libHexagon"$dllExt" ./Hexagon/Plugins/Hexagon"$dllExt"
cp -f "$localSourceCodeLocation/Hexagon/Hexagon_GUI/Build_Scripts/Windows/Installer/UPDATES AND SOURCE CODE.TXT" ./Hexagon

# Install Qt DLLs
if [ ${dllExt} == ".dll" ]; then
    qtpaths6Location=$(which qtpaths6.exe)
    qtDLLsLocation=${qtpaths6Location%/*}
    
    # Install Qt Plugins
    mkdir ./Hexagon/Qt
    echo [Paths] > ./Hexagon/qt.conf
    echo Plugins=./Qt >> ./Hexagon/qt.conf
    cp -rf /mingw64/share/qt6/plugins/* ./Hexagon/Qt
    
    # Install root Qt DLLs
    ldd ./Hexagon/Hexagon"$exeExt" | awk '{print $3}' > allDLLs.txt
    ldd ./Hexagon/Plugins/Hexagon"$dllExt" | awk '{print $3}' >> allDLLs.txt
    grep -v "/Windows" allDLLs.txt > nonWindows.txt
    sort nonWindows.txt | uniq > requiredDLLs.txt
    while IFS= read -r requiredDLL; do
        cp "$requiredDLL" ./Hexagon/
    done < requiredDLLs.txt
    rm allDLLs.txt nonWindows.txt requiredDLLs.txt
fi
echo Done!

# Clean up
echo ""; echo [5/5] Cleaning up...
rm -rf source/
echo Done!

# Create the Deployed Files folder
echo ""; echo "Deploying Files..."
mkdir ./Hexagon/Config

# Zip up Hexagon archive for users who don't want an installer
echo ""; echo "Creating 7zip archive..."
mv ./Hexagon/ "./Hexagon $version"
7z a "./Hexagon $version.7z" "./Hexagon $version"
mv "./Hexagon $version" ./Hexagon/

# Windows Setup.exe section
#======================================================================================================================#
if [ ${MSYSTEM} == "MINGW64" ]; then
    # Prepare Installer for Compilation
    echo ""; echo "Compiling Hexagon Installer..."
    startRmdirSectionMarker="# --------------- BEGIN AUTO-GENERATED RMDIR SECTION --------------- #"
    endRmdirSectionMarker="# ---------------- END AUTO-GENERATED RMDIR SECTION ---------------- #"
    startFileSectionMarker="# --------------- BEGIN AUTO-GENERATED FILE SECTION ---------------- #"
    endFileSectionMarker="# ---------------- END AUTO-GENERATED FILE SECTION ----------------- #"

    startDeleteSectionMarker="# -------------- BEGIN AUTO-GENERATED DELETE SECTION --------------- #"
    endDeleteSectionMarker="# --------------- END AUTO-GENERATED DELETE SECTION ---------------- #"
    startUninstallSectionMarker="# ------------- BEGIN AUTO-GENERATED UNINSTALL SECTION ------------- #"
    endUninstallSectionMarker="# -------------- END AUTO-GENERATED UNINSTALL SECTION -------------- #"

    installerFile="$localSourceCodeLocation"/Hexagon/Hexagon_GUI/Build_Scripts/Windows/Installer/Hexagon.nsi
    rm rmdirSection.txt fileSection.txt deleteSectionUnsorted.txt deleteSection.txt uninstallSectionUnsorted.txt uninstallSection.txt > /dev/null 2>&1

    # Prepare RMDIR Section
    find ./Hexagon -maxdepth 1 -type d -print > directories.txt
    sed -i 's|/|\\|g' directories.txt
    sed -i 's|\.\\Hexagon|\$INSTDIR|g' directories.txt
    sed -i '/\\Config/d' directories.txt # don't delete config directories
    while IFS= read -r directory; do
        echo RmDir /r \""$directory"\" >> rmdirSection.txt
    done < directories.txt
    sed -i '/\"$INSTDIR\"/d' directories.txt # don't delete the base directory
    sed -i '/^RmDir \/r "$INSTDIR"$/d' rmdirSection.txt

    # Prepare File Section
    deployLocation="$(pwd)/Hexagon"
    find ./Hexagon -type d -print > directories.txt
    while IFS= read -r directory; do
        echo SetOutPath \""$directory"\" >> fileSection.txt
        while IFS= read -r -d $'\0' file; do
            echo File \""$file"\" >> fileSection.txt
        done < <(find "$directory" -maxdepth 1 -type f -print0)
    done < directories.txt
    sed -i 's|SetOutPath \"\./Hexagon|\SetOutPath \"$INSTDIR|g' fileSection.txt
    sed -i 's|File \"./Hexagon/|File \"'"$deployLocation"'/|g' fileSection.txt

    # Prepare Delete Section
    find ./Hexagon -type f -print > files.txt
    sed -i 's|/|\\|g' files.txt
    sed -i 's|\.\\Hexagon|$INSTDIR|g' files.txt
    while IFS= read -r file; do
        echo Delete \""$file"\" >> deleteSectionUnsorted.txt
    done < files.txt
    sed -i '/Hexagon\.exe/d' deleteSectionUnsorted.txt # don't delete the Hexagon exe for now
    awk '{ print length($0) " " $0; }' deleteSectionUnsorted.txt | sort -n -r | cut -d ' ' -f 2- > deleteSection.txt # sort from longest to shortest

    # Prepare Uninstall Section
    find ./Hexagon -type d -print > directories.txt
    sed -i 's|/|\\|g' directories.txt
    sed -i 's|\.\\Hexagon|\$INSTDIR|g' directories.txt
    while IFS= read -r directory; do
        echo RmDir /r \""$directory"\" >> uninstallSectionUnsorted.txt
    done < directories.txt
    sed -i '/\"$INSTDIR\"/d' directories.txt # don't delete the base directory
    sed -i '/^RmDir \/r "$INSTDIR"$/d' uninstallSectionUnsorted.txt
    awk '{ print length($0) " " $0; }' uninstallSectionUnsorted.txt | sort -n -r | cut -d ' ' -f 2- > uninstallSection.txt # sort from longest to shortest

    convert_unix_paths_to_windows() {
        sed -i 's|/a/|A:/|g; s|/b/|B:/|g; s|/c/|C:/|g; s|/d/|D:/|g; s|/e/|E:/|g; s|/f/|F:/|g; s|/g/|G:/|g; s|/h/|H:/|g; s|/i/|I:/|g; s|/j/|J:/|g; s|/k/|K:/|g; s|/l/|L:/|g; s|/m/|M:/|g; s|/n/|N:/|g; s|/o/|O:/|g; s|/p/|P:/|g; s|/q/|Q:/|g; s|/r/|R:/|g; s|/s/|S:/|g; s|/t/|T:/|g; s|/u/|U:/|g; s|/v/|V:/|g; s|/w/|W:/|g; s|/x/|X:/|g; s|/y/|Y:/|g; s|/z/|Z:/|g; s|/|\\|g' "$1"
    }

    convert_unix_paths_to_windows "fileSection.txt"
    convert_unix_paths_to_windows "deleteSection.txt"

    update_section() {
        # Find the line numbers of the markers
        startLine=$(grep -n "$1" "$installerFile" | cut -d ':' -f 1)
        endLine=$(grep -n "$2" "$installerFile" | cut -d ':' -f 1)

        # Check if markers were found
        if [ -z "$startLine" ] || [ -z "$endLine" ]; then
          echo "Markers not found in $installerFile"
          exit 1
        fi

        # Calculate the line range to delete
        deleteRange="$((startLine+1)),${endLine}d"
        
        # Restore the end section marker
        echo $2 >> $3

        # Delete lines between markers and insert contents of fileSection.txt
        sed -i "$deleteRange" "$installerFile"
        sed -i "${startLine}r $3" "$installerFile"
    }

    update_section "$startRmdirSectionMarker" "$endRmdirSectionMarker" "rmdirSection.txt"
    update_section "$startFileSectionMarker" "$endFileSectionMarker" "fileSection.txt"
    update_section "$startDeleteSectionMarker" "$endDeleteSectionMarker" "deleteSection.txt"
    update_section "$startUninstallSectionMarker" "$endUninstallSectionMarker" "uninstallSection.txt"
    rm rmdirSection.txt fileSection.txt deleteSectionUnsorted.txt deleteSection.txt uninstallSectionUnsorted.txt uninstallSection.txt directories.txt files.txt

    convert_to_windows_path() {
        echo "$1" | sed 's|/a/|A:/|g; s|/b/|B:/|g; s|/c/|C:/|g; s|/d/|D:/|g; s|/e/|E:/|g; s|/f/|F:/|g; s|/g/|G:/|g; s|/h/|H:/|g; s|/i/|I:/|g; s|/j/|J:/|g; s|/k/|K:/|g; s|/l/|L:/|g; s|/m/|M:/|g; s|/n/|N:/|g; s|/o/|O:/|g; s|/p/|P:/|g; s|/q/|Q:/|g; s|/r/|R:/|g; s|/s/|S:/|g; s|/t/|T:/|g; s|/u/|U:/|g; s|/v/|V:/|g; s|/w/|W:/|g; s|/x/|X:/|g; s|/y/|Y:/|g; s|/z/|Z:/|g; s|/|\\\\|g'
    }

    # Update the Installer version
    sed -i "s/!define VERSION \".*/!define VERSION \"$installerVersion\"/g" "$installerFile"

    # Update the license location
    cp -f "$localSourceCodeLocation/Hexagon/LICENSE" "./LICENSE.txt"
    licenseLocation=$(convert_to_windows_path "$(pwd)/LICENSE.txt")
    sed -i "s|^!define LICENSE_TXT \".*\"|!define LICENSE_TXT \"$licenseLocation\"|" "$installerFile"

    # Update the installer output location
    installerOutputLocation=$(convert_to_windows_path "$(pwd)/Hexagon.$version.Setup.exe")
    sed -i "s|^!define INSTALLER_NAME \".*\"|!define INSTALLER_NAME \"$installerOutputLocation\"|" "$installerFile"

    # Compile the Hexagon installer for users who want an installer
    makensis "$installerFile"
    rm "./LICENSE.txt"
fi

echo "Done!"; echo ""; echo "Deploy complete! Enjoy Hexagon!"
exit 0
