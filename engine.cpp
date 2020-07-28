#include "engine.h"

Engine::Engine()
{
    this->luaEngine = nullptr;
}

bool Engine::New(){
    if(this->luaEngine)
        this->Close();
    this->luaEngine = luaL_newstate();
    if(!this->luaEngine)
        return false;
    luaL_openlibs(this->luaEngine);
    return true;
}

void Engine::Close() {
    if(this->luaEngine) {
        lua_close(this->luaEngine);
        this->luaEngine = nullptr;
    }
}

const char* Engine::Run(const char* code) {
    luaL_dostring(this->luaEngine, code);
    return lua_tostring(this->luaEngine, -1);
}

const char* Engine::Run(const QString code) {
    QString mainCode = QString("%1\nreturn onTick()").arg(code);
    return Run(mainCode.toLocal8Bit().data());
}
