#include "level_editor_tab.h"
#include <string>
#include <iostream>
#include <sstream>
#include "../console/console.h"
#include <SDL3/SDL.h>
#include "../json/json_variant.h"
#include "program_state.h"
#include "../imgui/imgui.h"
#include "../imgui/misc/cpp/imgui_stdlib.h"
#include "im_vec.h"

namespace UFOEngineStudio{

LevelEditorTab::LevelEditorTab(std::string _file){
    name = _file.substr(_file.find_last_of("/")+1);
}

void LevelEditorTab::OnActive(ProgramState* _program_state){
    ImGui::Begin("Layers");

    ImGui::ImageButton("Icon", (ImTextureID)(intptr_t)_program_state->example_texture, ImVec2(_program_state->example_texture->w, _program_state->example_texture->h));

    static std::string s;
    static std::string t;
    ImGui::InputText("Actors",&s);
    ImGui::InputText("Tiles",&t);
    //ImGui::ImageButton("image",);
    ImGui::Button("Add Layer [+]");

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

}