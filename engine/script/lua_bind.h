#pragma once
#include <string>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace MoLin {

class LuaBind {
public:
    LuaBind();
    ~LuaBind();
    bool DoFile(const std::string& filename);
    bool DoString(const std::string& code);
    lua_State* GetState() { return L; }
    void RegisterCoreFunctions();
private:
    lua_State* L = nullptr;
};

} // namespace MoLin
