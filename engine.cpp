#include "engine.h"

Engine::Engine()
{
    this->lua = nullptr;
}

bool Engine::New(){
    if(this->lua)
        this->Close();
    this->lua = luaL_newstate();
    if(!this->lua)
        return false;
    luaL_openlibs(this->lua);
    return true;
}

void Engine::Close() {
    if(this->lua) {
        lua_close(this->lua);
        this->lua = nullptr;
    }
}

const char* Engine::Run(const char* code) {
    luaL_dostring(this->lua, code);
    return lua_tostring(this->lua, -1);
}
