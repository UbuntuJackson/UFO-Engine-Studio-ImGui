#pragma once

#include "tab.h"
#include "../imgui/imgui.h"
#include "dock_utils.h"
#include "im_vec.h"
#include "file.h"

namespace UFOEngineStudio{

class AssetBrowserTab : public Tab{
public:
    AssetBrowserTab(std::string _name) : Tab(){
        name = _name;
    }

    void OnActive(ImGuiID _local_dockspace_id, ProgramState *_program){

        ImGui::Begin("Assets");

        if(ImGui::IsItemClicked()) for(auto&& [asset_path, asset] : _program->asset_manager.textures) asset->edit_mode = false;

        if(ImGui::BeginDragDropTarget() && ImGui::IsMouseReleased(ImGuiMouseButton_Left)){
            const ImGuiPayload* payload = ImGui::GetDragDropPayload();

            if(payload->DataType == std::string("FROM_FILE")){
                FileNode* file = (FileNode*)payload->Data;

                if(file->file_name.find(".") != file->file_name.npos){
                    Console::PrintLine("found dot");
                    if(file->file_name.substr(file->file_name.find_last_of(".")+1) == "png"){
                        TreeFile* tree_file = (TreeFile*)payload->Data;
                        _program->asset_manager.AddTexture(tree_file->path_for_drag_drop_payload_use_only, tree_file->path_for_drag_drop_payload_use_only);
                    }
                }
            }
        }

        int unique_id = 0;

        std::string remove_with_path = "";
        bool should_remove_alias = false;

        for(auto&& [asset_path, asset] : _program->asset_manager.textures){

            float max_width = 100.0f;
            float scale = 1.0f;

            if(asset->texture->w > max_width) scale = max_width/asset->texture->w; 

            bool pressed = ImGui::ImageButton(
                ("###ClassIconUniqueEntry"+asset->alias).c_str(),
                (ImTextureID)(intptr_t)asset->texture,
                ImVec2(asset->texture->w*scale, asset->texture->h*scale));

            ImGui::SameLine();

            if(asset->edit_mode) ImGui::InputText(("###"+std::to_string(unique_id)).c_str(),&asset->alias);
            else ImGui::TextUnformatted(asset->alias.c_str());

            if(ImGui::IsItemClicked()){
                for(auto&& [asset_path, asset] : _program->asset_manager.textures) asset->edit_mode = false;
                asset->edit_mode = !asset->edit_mode;
            }

            ImGui::SameLine();

            if(ImGui::Button(std::string("[x]###Close"+std::to_string(unique_id)).c_str())){
                remove_with_path = asset_path;
                should_remove_alias = true;
            }

            

            unique_id++;
        }

        if(should_remove_alias) _program->asset_manager.DeleteTexture(remove_with_path);

        ImGui::End();

    }

    void
    OnMakeDockSpace(ImGuiID _local_dockspace_id, ProgramState* _program_state){
        ImGuiDockSpaceFill(_local_dockspace_id, ImGui::GetWindowSize(), "Assets");
    }

};

}