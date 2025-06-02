#include "file_node.h"
#include "../imgui/imgui.h"
#include "../imgui/misc/cpp/imgui_stdlib.h"
#include "directory.h"
#include "program_state.h"
#include "file.h"
#include "../file/file.h"

namespace UFOEngineStudio{

void Directory::Update(int _file_index, Directory* _parent,std::string path , ProgramState* _program){

    bool folder_opened = ImGui::TreeNodeEx(editing_name ? ("###Directory"+std::to_string(id)).c_str() : (file_name+"###Directory"+std::to_string(id)).c_str());

    if(editing_name){
        ImGui::SameLine();
        
        ImGui::InputText(("###EditText"+std::to_string(id)).c_str(),&file_name);

        if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemHovered()){
            editing_name = false;
        }

        if(ImGui::IsKeyPressed(ImGuiKey_Enter)){
            editing_name = false;
        }
    }

    if(ImGui::IsItemClicked()){
        _program->drag_drop_stack.push_back(DragDrop{this, _parent});
    }

    if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)){
        ImGui::SetDragDropPayload("CONTENT_BROWSER_DATA", this, sizeof(this));

        ImGui::EndDragDropSource();
    }

    //This is in case file is dropped on folder
    if(ImGui::BeginDragDropTarget()){

        if(ImGui::IsMouseReleased(ImGuiMouseButton_Left)){
            Console::PrintLine("Drop");
            _program->drag_drop_stack.back().move_to_folder = this;
            //_program->drag_drop_stack.push_back();
        }
        //ImGui::AcceptDragDropPayload("CONTENT_BROWSER_DATA");

        ImGui::EndDragDropTarget();
    }

    if(ImGui::BeginPopupContextItem(("Options"+std::to_string(id)).c_str())){
        if(ImGui::MenuItem("Rename")){
            editing_name = true;
        }
        if(ImGui::MenuItem("Delete")){
            to_be_deleted = true;
            Console::PrintLine(file_name);
        }
        if(ImGui::MenuItem("New File")){
            file_nodes_to_be_added_at_end_of_frame.push_back(std::make_unique<TreeFile>(true));
            file_nodes_to_be_added_at_end_of_frame.back()->editing_name = true;
            
        }
        if(ImGui::MenuItem("New Folder")){
            
            Console::PrintLine(file_name);
        }
        ImGui::EndPopup();
    }

    if(folder_opened){

        for(int index = 0; index < file_nodes.size(); index++){
            file_nodes[index]->Update(index,this,(_parent ? path+"/" : "")+file_name,_program);
        }

        ImGui::TreePop();
        
    }

    FileNode::Update(_file_index, _parent,path, _program);

}

}