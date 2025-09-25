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

    if(ImGui::BeginTabItem(((DetermineIfEdited() ? name : name+"*")+"###"+path).c_str(), &opened, ImGuiTabItemFlags_None)){
        //ImGui::BeginChildFrame(1,ImVec2(800,600));
        //ImGui::Begin(("TabWindow##"+name).c_str());
        ImGuiID local_dockspace_id = ImGui::GetID(name.c_str());

        OnMakeDockSpace(local_dockspace_id, _program_state);

        ImGui::DockSpace(local_dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        //ImGui::End();

        _program_state->active_tab = this;

        OnActive(local_dockspace_id ,_program_state);

        //ImGui::EndChildFrame();
        ImGui::EndTabItem();
        
    }
    
}

bool Tab::DetermineIfEdited(){
    return false;
}

void Tab::OnMakeDockSpace(ImGuiID _local_dockspace_id, ProgramState* _program_state){
    
}

void Tab::OnActive(ImGuiID _local_dockspace_id , ProgramState* _program_state){}

void Tab::OnSave(ProgramState* _program_state){
}

}