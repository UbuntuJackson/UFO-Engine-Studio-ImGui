#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include "../UFO-Engine-GL/imgui/imgui.h"

namespace UFOEngineStudio{

class Editor;

class Tab{
public:
    std::string name;
    std::string path;
    bool opened = true;
    bool is_edited = false;

    virtual void Update(Editor* _program_state);
    virtual bool DetermineIfEdited();
    virtual void OnActive(ImGuiID _local_dockspace_id, Editor* _program_state);
    virtual void OnMakeDockSpace(ImGuiID _local_dockspace_id, Editor* _program_state);

    virtual void OnSave(Editor* _program_state);

};

}