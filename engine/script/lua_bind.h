#pragma once
#include <string>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace MoLin {
namespace Script {

class LuaBind {
public:
    LuaBind();
    ~LuaBind();

    bool DoFile(const std::string& filename);
    bool DoString(const std::string& code);
    void RegisterCoreFunctions();

    lua_State* GetState() { return L; }

private:
    lua_State* L;
};

} // namespace Script
} // namespace MoLin
