#include <string>
#include <iostream>
#include <sstream>
#include "../console/console.h"
#include <SDL3/SDL.h>
#include "../json/json_variant.h"
#include "../UFO-Engine-GL/imgui/imgui.h"
#include "tab.h"
#include "editor.h"
#include "im_vec.h"

namespace UFOEngineStudio{

void Tab::Update(Editor* _editor){

    if(ImGui::BeginTabItem(((DetermineIfEdited() ? name : name+"*")+"###"+path).c_str(), &opened, ImGuiTabItemFlags_None)){
        //ImGui::BeginChildFrame(1,ImVec2(800,600));
        //ImGui::Begin(("TabWindow##"+name).c_str());
        ImGuiID local_dockspace_id = ImGui::GetID(name.c_str());

        OnMakeDockSpace(local_dockspace_id, _editor);

        ImGui::DockSpace(local_dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        //ImGui::End();

        _editor->active_tab = this;

        OnActive(local_dockspace_id ,_editor);

        //ImGui::EndChildFrame();
        ImGui::EndTabItem();
        
    }
    
}

bool Tab::DetermineIfEdited(){
    return false;
}

void Tab::OnMakeDockSpace(ImGuiID _local_dockspace_id, Editor* _program_state){
    
}

void Tab::OnActive(ImGuiID _local_dockspace_id , Editor* _program_state){}

void Tab::OnSave(Editor* _program_state){
}

}