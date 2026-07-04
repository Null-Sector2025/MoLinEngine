# 墨麟引擎预编译依赖库

## 目录结构
```

libs/
├── linux/
│   ├── x64/          # Linux x86_64 的 SDL2/Lua 静态库
│   └── arm64/        # Linux ARM64 的 SDL2/Lua 动态库
├── windows/
│   └── x64/          # Windows x64 的 SDL2/Lua 静态库和 DLL
├── android/
│   ├── arm64-v8a/    # Android ARM64 的 SDL2/Lua .so 文件
│   └── include/      # 头文件
└── README.md

```

## 如何获取依赖库
使用 GitHub Actions 构建产物中的依赖包，解压到对应目录即可。

## 当前状态
依赖库需通过 GitHub Actions 的 `build-deps` 任务生成。
