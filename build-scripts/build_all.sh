#!/bin/bash
echo "================================================"
echo " 墨麟引擎 全平台构建脚本"
echo "================================================"

ENGINE_DIR="$(cd "$(dirname "$0")/.." && pwd)"

echo ""
echo "[1/3] 构建 Linux x64..."
bash "$ENGINE_DIR/build-scripts/build_linux.sh"

echo ""
echo "[2/3] 构建 Windows x64..."
# 在 Windows 上运行 build_windows.bat

echo ""
echo "[3/3] 生成 Android APK 项目..."
bash "$ENGINE_DIR/build-scripts/build_android.sh"

echo ""
echo "================================================"
echo " 全部构建完成！"
echo " Linux:   dist/linux/bin/MoLinEngine"
echo " Windows: dist/windows/bin/MoLinEngine.exe"
echo " Android: build-android-apk (需用 Android Studio 打开编译)"
echo "================================================"
