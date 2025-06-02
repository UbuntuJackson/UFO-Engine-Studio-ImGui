#include <string>
#include <iostream>
#include <sstream>
#include "../console/console.h"
#include <SDL3/SDL.h>
#include "../json/json_variant.h"
#include "program_state.h"
#include "../imgui/imgui.h"
#include "../imgui/misc/cpp/imgui_stdlib.h"
#include "tab.h"
#include "im_vec.h"

namespace UFOEngineStudio{

void Tab::Update(ProgramState* _program_state){

    if(ImGui::BeginTabItem(((DetermineIfEdited() ? name : name+"*")+"###"+name).c_str(), &opened, ImGuiTabItemFlags_None)){
        //ImGui::BeginChildFrame(1,ImVec2(800,600));
        ImGuiID dock_space_id = ImGui::GetID(name.c_str());

        ImGui::DockSpace(dock_space_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        
        _program_state->active_tab = this;

        OnActive(_program_state);
        
        //ImGui::EndChildFrame();
        ImGui::EndTabItem();
        
    }
    
}

bool Tab::DetermineIfEdited(){
    return false;
}

void Tab::OnActive(ProgramState* _program_state){}

void Tab::OnSave(){
}

}