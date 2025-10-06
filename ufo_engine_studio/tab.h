#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include "../UFO-Engine-GL/imgui/imgui.h"
#include <graphics.h>

namespace UFOEngineStudio{

class Editor;

class Tab{
public:

    int id = 0;

    static inline int id_counter = 0;

    std::string name;
    std::string path;
    bool opened = true;
    bool is_edited = false;
    std::string name_and_imgui_id;
    Tab();

    virtual void Update(Editor* _program_state, float _delta_time);
    virtual bool DetermineIfEdited();
    virtual void OnActive(ImGuiID _local_dockspace_id, Editor* _program_state, float _delta_time);
    virtual void OnMakeDockSpace(ImGuiID _local_dockspace_id, Editor* _program_state);

    virtual void OnSave(Editor* _program_state);

    virtual void LevelUpdatePhase(float _float){

    }

    virtual void LevelDrawPhase(ufo::Graphics* _graphics){

    }

};

}