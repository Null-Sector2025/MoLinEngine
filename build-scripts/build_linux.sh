#!/bin/bash
echo "========================================"
echo " 墨麟引擎 Linux 可执行文件构建脚本"
echo "========================================"

ENGINE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
LIBS_DIR="$ENGINE_DIR/libs/linux/x64"
BUILD_DIR="$ENGINE_DIR/build-linux-exe"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH="$LIBS_DIR" \
  -DBUILD_SHARED_LIBS=OFF \
  -DCMAKE_INSTALL_PREFIX="$ENGINE_DIR/dist/linux"

make -j$(nproc)
make install

echo ""
echo "构建完成！输出目录: $ENGINE_DIR/dist/linux"
echo "可执行文件: dist/linux/bin/MoLinEngine"
