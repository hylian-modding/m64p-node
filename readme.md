#Windows

## Install required nodejs global modules

npm install -g typescript cmake-js

## Install required C++ libraries via vcpkg

git clone https://github.com/microsoft/vcpkg vcpkg_ml64
cd vcpkg_ml64
bootstrap-vcpkg.bat
vcpkg install sdl2 sdl2-ttf sdl2-image freetype glew

## Build the addon

set vcpkg_x86=<full path to vcpkg_ml64>\installed\x86-windows
git clone https://github.com/hylian-modding/m64p-node
cd m64p-node
npm install
npm run dist

m64p-node\dist contains the module and its typescript definition
<full path to vcpkg_ml64>\installed\x86-windows\bin contains the dependencies DLLs

#Linux

sudo apt install build-essential cmake nodejs npm nasm libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev libfreetype-dev libglew-dev libsamplerate0-dev libspeexdsp-dev libpng-dev libz-dev libgtk-3-dev
npm install -g typescript cmake-js
git clone https://github.com/hylian-modding/m64p-node
cd m64p-node
npm install
npm run dist