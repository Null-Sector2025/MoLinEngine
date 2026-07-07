@echo off
echo ========================================
echo  墨麟引擎 Windows EXE 构建脚本
echo ========================================

set ENGINE_DIR=%~dp0..
set LIBS_DIR=%ENGINE_DIR%\libs\windows\x64
set BUILD_DIR=%ENGINE_DIR%\build-windows-exe

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd /d "%BUILD_DIR%"

:: 使用本地预编译的 SDL2 和 Lua 库
cmake .. ^
  -G "Visual Studio 17 2022" ^
  -A x64 ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DSDL2_DIR="%LIBS_DIR%" ^
  -DSDL2_image_DIR="%LIBS_DIR%" ^
  -DSDL2_mixer_DIR="%LIBS_DIR%" ^
  -DSDL2_ttf_DIR="%LIBS_DIR%" ^
  -DLUA_DIR="%LIBS_DIR%" ^
  -DBUILD_SHARED_LIBS=OFF ^
  -DCMAKE_INSTALL_PREFIX="%ENGINE_DIR%\dist\windows"

cmake --build . --config Release --target install

echo.
echo 构建完成！输出目录: %ENGINE_DIR%\dist\windows
echo 可执行文件: dist\windows\bin\MoLinEngine.exe
pause
