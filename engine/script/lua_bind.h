#pragma once
#include <string>
#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>
#include <lua5.4/lauxlib.h>

namespace MoLin {

class LuaBind {
public:
    LuaBind();
    ~LuaBind();

    bool DoFile(const std::string& filename);
    bool DoString(const std::string& code);
    lua_State* GetState() { return L; }

    // 暴露给 Lua 的 C++ 函数可在此注册
    void RegisterCoreFunctions();

private:
    lua_State* L = nullptr;
};

} // namespace MoLin
