#ifndef ENGINE_H
#define ENGINE_H

#include <string>

extern "C" {
#include "lua535/lua.h"
#include "lua535/lauxlib.h"
#include "lua535/lualib.h"
}

class Engine
{
public:
    Engine();
    bool New();
    void Close();
    const char* Run(const char* code);
private:
    lua_State *lua;
};

#endif // ENGINE_H
