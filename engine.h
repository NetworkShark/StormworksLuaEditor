#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <QString>

extern "C" {
#include "lua540/lua.h"
#include "lua540/lauxlib.h"
#include "lua540/lualib.h"
}

typedef struct luaMemFile
{
  const char *text;
  size_t size;
} luaMemFile;

class Engine
{
public:
    Engine();
    bool New();
    void Close();
    const char* Run(const char* code);
    const char* Run(const QString code);
private:
    lua_State *luaEngine;
//    const char *readMemFile(lua_State *, void *ud, size_t *size);
};

#endif // ENGINE_H
