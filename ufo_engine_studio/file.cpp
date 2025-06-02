#include "../imgui/imgui.h"
#include "../imgui/misc/cpp/imgui_stdlib.h"
#include "file_node.h"
#include "file.h"
#include "directory.h"
#include "program_state.h"
#include "text_editor_tab.h"
#include "../file/file.h"

namespace UFOEngineStudio{

    void TreeFile::Update(int _file_index, Directory* _parent,std::string path , ProgramState* _program){

        if(editing_name){
            ImGui::InputText(("##FileName"+std::to_string(id)).c_str(),&file_name);
        }
        else{
            
            ImGui::Text(file_name.c_str());

            if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered()){

                std::string tab_name = path+"/"+file_name.substr((path+"/"+file_name).find_last_of("/")+1);

                bool tab_already_open = false;

                for(auto&& tab : _program->tabs){
                    if(tab->name == tab_name){
                        tab_already_open = true;
                    }
                }

                if(!tab_already_open){

                    std::string p = _program->working_directory_path+path+"/"+file_name;

                    if(File::Exists(p)){
                        File f;
                        f.Read(p);

                        auto u_tf = std::make_unique<TextEditorTab>(path+"/"+file_name, f.GetAsString());
                        u_tf->path = p;
                        
                        _program->tabs.push_back(std::move(u_tf));
                    }
                    else{
                        Console::PrintLine("File does not exist at:",p);
                    }
                
                }
            }

            if(ImGui::IsItemClicked()){
                Console::PrintLine("This file is on path",path+"/"+file_name);
                _program->drag_drop_stack.push_back(DragDrop{this, _parent});
            }
    
            //If thing is dropped on a file
            if(ImGui::BeginDragDropTarget()){

                if(ImGui::IsMouseReleased(ImGuiMouseButton_Left)){
                    Console::PrintLine("Drop");

                    //Move to parent folder to place next to neighbouring folder
                    _program->drag_drop_stack.back().move_to_folder = _parent;
                    _program->drag_drop_stack.back().index_in_move_to_folder = _file_index;
                    //_program->drag_drop_stack.push_back();
                }
                //ImGui::AcceptDragDropPayload("CONTENT_BROWSER_DATA");
    
                ImGui::EndDragDropTarget();
            }

            if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)){
                ImGui::SetDragDropPayload("CONTENT_BROWSER_DATA", this, sizeof(this));
    
                ImGui::EndDragDropSource();
            }
        }

        /*if(ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !editing_name){
            editing_name = !editing_name;
        }*/

        if(!ImGui::IsItemClicked() && (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsKeyPressed(ImGuiKey_Enter))){
            editing_name = false;

            if(is_new_file){
                File f = File::New();
                f.Insert("");
                f.Write(_program->working_directory_path + path+"/"+file_name);
            
                is_new_file = false;
            }
        }

        if(ImGui::BeginPopupContextItem(("Options"+std::to_string(id)).c_str())){
            if(ImGui::MenuItem("Rename")){
                editing_name = true;
            }
            if(ImGui::MenuItem("Delete")){
                to_be_deleted = true;
            }
            ImGui::EndPopup();
        }

        FileNode::Update(_file_index, _parent,path, _program);

    }

}