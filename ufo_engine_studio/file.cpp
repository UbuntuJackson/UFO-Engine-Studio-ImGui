#include "../imgui/imgui.h"
#include "../imgui/misc/cpp/imgui_stdlib.h"
#include "file_node.h"
#include "file.h"
#include "directory.h"
#include "program_state.h"
#include "text_editor_tab.h"
#include "../file/file.h"
#include "level_editor_tab.h"
#include <filesystem>

namespace UFOEngineStudio{

    void TreeFile::Update(int _file_index, Directory* _parent,std::string path , ProgramState* _program){

        if(editing_name){
            ImGui::InputText(("###EditText"+std::to_string(id)).c_str(),&file_name);

            //This is just duplicate code from this file to make the actual file you just input
            if(!ImGui::IsItemClicked() && (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsKeyPressed(ImGuiKey_Enter))){
                editing_name = false;

                if(is_new_file){
                    File f = File::New();
                    f.Insert("");
                    f.Write(_program->working_directory_path + path+"/"+file_name);
                
                    is_new_file = false;
                }

                _program->should_refresh_working_directory = true;
            }
        }
        else{
            
            ImGui::Text(file_name.c_str());

            if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered()){

                std::string tab_name = (path+"/"+file_name).substr((path+"/"+file_name).find_last_of("/")+1);
                std::string full_path = _program->working_directory_path+path+"/"+file_name;

                bool tab_already_open = false;

                for(auto&& tab : _program->tabs){
                    if(tab->path == full_path){
                        tab_already_open = true;
                    }
                }

                if(!tab_already_open){

                    if(File::Exists(full_path)){
                        std::string extension = tab_name.substr(tab_name.find_last_of(".")+1);

                        if(extension == "cpp" || extension == "h" || extension == "hpp" || extension == "h"){
                            File f;
                            f.Read(full_path);

                            auto u_tf = std::make_unique<TextEditorTab>(path+"/"+file_name, f.GetAsString());
                            u_tf->path = full_path;
                            
                            _program->tabs.push_back(std::move(u_tf));
                        }
                        else if(extension == "ason"){
                            JsonDictionary d = JsonVariant::Read(full_path);

                            auto u_level_editor_tab = std::make_unique<LevelEditorTab>(_program, "");
                            u_level_editor_tab->actor->ReadFromJson(&d);
                            u_level_editor_tab->name = file_name;
                            u_level_editor_tab->path = full_path;
                            _program->tabs.push_back(std::move(u_level_editor_tab));
                            
                        }
                        else{
                            Console::PrintLine("File extension not supported:",full_path);
                        }
                    }
                    else{
                        Console::PrintLine("File does not exist at:",full_path);
                    }
                
                }
            }

            if(ImGui::IsItemClicked()){
                
                _program->drag_drop_stack.push_back(DragDrop{this, _parent, _program->working_directory_path + path});
            }
    
            //If thing is dropped on a file
            if(ImGui::BeginDragDropTarget()){

                if(ImGui::IsMouseReleased(ImGuiMouseButton_Left)){

                    _program->should_refresh_working_directory = true;

                    //Move to parent folder to place next to neighbouring folder
                    _program->drag_drop_stack.back().move_to_folder = _parent;
                    _program->drag_drop_stack.back().move_to_path = _program->working_directory_path + path;
                    _program->drag_drop_stack.back().index_in_move_to_folder = _file_index;
                    //_program->drag_drop_stack.push_back();
                }
                //ImGui::AcceptDragDropPayload("CONTENT_BROWSER_DATA");
    
                ImGui::EndDragDropTarget();
            }

            if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)){

                path_for_drag_drop_payload_use_only = path+"/"+file_name;

                ImGui::SetDragDropPayload("FROM_FILE", this, sizeof(*this));
    
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
                TurnOnEditMode();
            }
            if(ImGui::MenuItem("Delete")){
                std::string full_path = _program->working_directory_path +"/"+ path+"/"+file_name;
                int res = std::remove(full_path.c_str());
                if(res) Console::PrintLine("TreeFile::Update(): Failture upon trying to remove", full_path.c_str());
                to_be_deleted = true;
                
            }
            if(ImGui::MenuItem("New File")){
                _parent->file_nodes_to_be_added_at_end_of_frame.push_back(std::make_unique<TreeFile>(true));
                _parent->file_nodes_to_be_added_at_end_of_frame.back()->editing_name = true;
                
            }
            if(ImGui::MenuItem("New Folder")){
                
                std::string full_path = _program->working_directory_path + path+"/NewFolder";
                std::filesystem::create_directory(full_path);
                
                _parent->file_nodes_to_be_added_at_end_of_frame.push_back(std::make_unique<Directory>(true));
                _parent->file_nodes_to_be_added_at_end_of_frame.back()->file_name = "NewFolder";
                _parent->file_nodes_to_be_added_at_end_of_frame.back()->TurnOnEditMode();
            }
            ImGui::EndPopup();
        }

        FileNode::Update(_file_index, _parent,path, _program);

    }

}