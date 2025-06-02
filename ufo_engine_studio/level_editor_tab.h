#pragma once
#include "tab.h"

namespace UFOEngineStudio{

class ProgramState;

class LevelEditorTab : public Tab{
public:
    LevelEditorTab(std::string _file);

    void OnActive(ProgramState* _program_state);
};

}