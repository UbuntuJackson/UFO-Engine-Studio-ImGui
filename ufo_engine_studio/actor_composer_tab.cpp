#include <memory>
#include <SDL3/SDL.h>
#include "../imgui/imgui.h"
#include "tab.h"
#include "actor_node.h"
#include "program_state.h"
#include "actor_composer_tab.h"
#include "program_state.h"
#include "main_menu_bar.h"
#include "dock_utils.h"

namespace UFOEngineStudio{

ActorComposerTab::ActorComposerTab(){
    actor = std::make_unique<ActorNode>();
}

void ActorComposerTab::OnActive(ImGuiID _local_dockspace_id, ProgramState* _program_state){

    ImGui::Begin("Actor Composition...");

    actor->Update(0, nullptr, "", _program_state);
    actor->DeleteActorNodesMarkedForDeletion();
    actor->AddActorNodesRecursive();

    ImGui::End();

}

void ActorComposerTab::OnMakeDockSpace(ImGuiID _local_dockspace_id, ProgramState* _program_state){
    ImGuiDockSpaceFill(_local_dockspace_id, ImGui::GetWindowSize(), "Actor Composition...");
}

void ActorComposerTab::OnSave(ProgramState* _program_state){
    if(path != "") actor->WriteToJson().Write(path);
    else{
        const char file_location[] = "/home";

        SDL_ShowSaveFileDialog(&OnNewActorFile , this, _program_state->window, nullptr, 0, file_location);
    }
}

}