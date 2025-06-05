#include "level_editor_tab.h"
#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include "../console/console.h"
#include <SDL3/SDL.h>
#include "../json/json_variant.h"
#include "program_state.h"
#include "../imgui/imgui.h"
#include "../imgui/misc/cpp/imgui_stdlib.h"
#include "im_vec.h"
#include "dock_utils.h"
#include "actor_node.h"

namespace UFOEngineStudio{

LevelEditorTab::LevelEditorTab(std::string _file){
    name = _file.substr(_file.find_last_of("/")+1);
    level_root = std::make_unique<ActorNode>();
    level_root->name = "LevelRoot";
}

void LevelEditorTab::OnActive(ImGuiID _local_dockspace_id , ProgramState* _program_state){

    if(actor_variant_config_open){
        ImGui::Begin(("Actor Variant Configuration###Configuration"+name).c_str());
        ImGui::End();
    }

    ImGui::Begin("LevelContentBrowser");

    ImGuiID level_content_browser_dock = ImGui::GetID("LevelContentBrowserDock");
    ImGuiDockSpaceSplit(level_content_browser_dock, ImGui::GetWindowSize(), "Instances", "Classes", SplitDirections::VERTICAL);
    ImGui::DockSpace(level_content_browser_dock, ImVec2(0.0f, 0.0f));

    ImGui::End();

    ImGui::Begin("Classes");

    std::string last_category;

    int unique_id_counter = 0;

    for(const auto& actor_variant : _program_state->project.actor_categories){
        if(last_category != actor_variant.category) ImGui::Text(("-- "+actor_variant.category + " --").c_str());

        if(last_category != actor_variant.category) ImGui::Separator();
        
        bool pressed = ImGui::ImageButton(("###ClassIconUniqueEntry"+std::to_string(unique_id_counter)).c_str(), (ImTextureID)(intptr_t)_program_state->example_texture, ImVec2(_program_state->example_texture->w, _program_state->example_texture->h));

        if(!actor_variant_config_open && pressed){
            actor_variant_config_open = true;
        }

        ImGui::SameLine();
        ImGui::Text(_program_state->project.actor_classes[actor_variant.class_id].name.c_str());

        last_category = actor_variant.category;
        unique_id_counter++;
    }

    ImGui::End();

    ImGui::Begin("Instances");

    level_root->Update(0 ,nullptr, "" , _program_state);
    level_root->DeleteActorNodesMarkedForDeletion();
    level_root->AddActorNodesRecursive();

    ImGui::End();

    ImGui::Text(name.c_str());

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar;

    ImGui::Begin("Level Grid", nullptr, window_flags);

    //The size of each tile
    int spacing_vertical_lines_along_x_axis = 16;
    int spacing_horizontal_lines_along_y_axis = 16;

    //Number of tiles in x- and y-axis
    int number_of_vertical_lines_along_x_axis = 50;
    int number_of_horizontal_lines_along_y_axis = 20;

    ImVec2 child_window_size = ImVec2(spacing_vertical_lines_along_x_axis * number_of_vertical_lines_along_x_axis, spacing_horizontal_lines_along_y_axis * number_of_horizontal_lines_along_y_axis);

    ImGui::BeginChild("Child",child_window_size);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    for(int xx = 0; xx < number_of_vertical_lines_along_x_axis; xx++){
        draw_list->AddLine(
            UFOEngineStudio::ImAdd(ImGui::GetWindowPos(), ImVec2(xx * spacing_vertical_lines_along_x_axis, 0)),
            UFOEngineStudio::ImAdd(ImGui::GetWindowPos(), ImVec2(xx * spacing_vertical_lines_along_x_axis, spacing_horizontal_lines_along_y_axis * number_of_horizontal_lines_along_y_axis)),
            ImColor(255,255,255,255) ,
            1.0f
        );
    }
    for(int yy = 0; yy < number_of_horizontal_lines_along_y_axis; yy++){
        draw_list->AddLine(
            UFOEngineStudio::ImAdd(ImGui::GetWindowPos(), ImVec2(0,                                                                           yy * spacing_horizontal_lines_along_y_axis)),
            UFOEngineStudio::ImAdd(ImGui::GetWindowPos(), ImVec2(spacing_vertical_lines_along_x_axis * number_of_vertical_lines_along_x_axis, yy * spacing_horizontal_lines_along_y_axis)),
            ImColor(255,255,255,255) ,
            1.0f
        );
    }

    ImGui::EndChild();

    ImGui::End();
    
}

void LevelEditorTab::OnMakeDockSpace(ImGuiID _local_dockspace_id, ProgramState* _program_state){
    ImGuiDockSpaceSplit(_local_dockspace_id, ImGui::GetWindowSize(), "Level Grid", "LevelContentBrowser", SplitDirections::HORIZONTAL);
}

}