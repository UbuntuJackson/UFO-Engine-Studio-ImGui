#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include "../console/console.h"
#include <SDL3/SDL.h>
#include "../json/json_variant.h"
#include "../imgui/imgui.h"

namespace UFOEngineStudio{

class ProgramState;

class Tab{
public:
    std::string name;
    std::string path;
    bool opened = true;
    bool is_edited = false;

    void Update(ProgramState* _program_state);
    virtual bool DetermineIfEdited();
    virtual void OnActive(ProgramState* _program_state);

    virtual void OnSave();

};

}