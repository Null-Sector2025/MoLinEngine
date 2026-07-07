# MoLinEngine - 2D RPG 引擎

## 依赖安装（Termux）
```bash
pkg install clang cmake make git sdl2 sdl2-image sdl2-mixer sdl2-ttf lua54 openal-soft libglvnd
编译

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON
make -j$(nproc)
```

运行示例

```bash
./MoLinEngineExe
```

使用 Lua 脚本

将 game.lua 放在 scripts/ 目录下，引擎会自动加载。
