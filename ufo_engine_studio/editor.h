#pragma once
#include <level.h>
#include <string>
#include "../UFO-Engine-GL/imgui/imgui.h"
#include "../ufo_engine_studio/dock_utils.h"
#include "file_node.h"

namespace UFOEngineStudio{

class Editor : public Level{
public:
    std::string opened_directory_path = "";
    bool should_refresh_working_directory = false;

    std::unique_ptr<FileNode> opened_directory = nullptr;

    void OpenFolder(std::string _path){
        opened_directory = FileNode::ParseFolder(_path);
        opened_directory->file_name = "";
        //opened_directory->id = -1;
        opened_directory_path = _path;
    }

    Editor(){
        OpenFolder("/home/uj/Documents/C++/blitbloot");
    }

    void ImportHeaderFileToProject(std::string _path){

    }

    void OnUpdate(float _delta_time){

        ImGuiWindowFlags im_gui_window_flags = ImGuiWindowFlags_NoDocking |
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;
            
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::Begin("DemoDockspaceForGodsSake", nullptr, im_gui_window_flags);

        ImGuiID dock_space_id = ImGui::GetID("DockSpaceOutsideTab");

        UFOEngineStudio::ImGuiDockSpaceSplit(dock_space_id, viewport->Size, "File Tree", "TabBarWindow", UFOEngineStudio::SplitDirections::HORIZONTAL);

        ImGui::DockSpace(dock_space_id, ImVec2(0.0f,0.0f), 0);

        ImGui::End();



        ImGui::Begin("File Tree");

        opened_directory->Update(0, nullptr, "", this);

        opened_directory->AddFileNodesRecursive();

        if(should_refresh_working_directory) OpenFolder(opened_directory_path);
        should_refresh_working_directory = false;

        ImGui::End();

        ImGui::Begin("TabBarWindow");

        ImGui::End();

        //UFOEngineStudio::ImGuiDockSpaceSplit(dock_space_id, viewport->Size, "File Tree", "TabBarWindow", UFOEngineStudio::SplitDirections::HORIZONTAL);

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {

                ImGui::EndMenu();
            }
            
            ImGui::EndMainMenuBar();
        }

    }
};

}