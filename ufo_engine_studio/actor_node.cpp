#include <string>
#include <memory>
#include "../imgui/imgui.h"
#include "../imgui/misc/cpp/imgui_stdlib.h"
#include "actor_node.h"
#include "program_state.h"
#include "../console/console.h"
#include "../json/json_variant.h"

namespace UFOEngineStudio{

ActorNode::ActorNode(){
    id = id_incrementation_counter++;
    name = "Actor#"+std::to_string(id);
}

void ActorNode::Update(int _file_index, ActorNode* _parent, std::string path , ProgramState* _program){
    bool folder_opened = ImGui::TreeNodeEx(editing_name ? ("###Directory"+std::to_string(id)).c_str() : (name+" ("+actor_type+")"+"###Directory"+std::to_string(id)).c_str());

    if(editing_name){
        ImGui::SameLine();
        
        ImGui::InputText(("###EditText"+std::to_string(id)).c_str(),&name);

        if(ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemHovered()){
            editing_name = false;
        }

        if(ImGui::IsKeyPressed(ImGuiKey_Enter)){
            editing_name = false;
        }
    }

    if(editing_type){
        ImGui::Begin("Change ActorNode Type");

        if(ImGui::Button("Actor")){
            actor_type = "Actor";
            editing_type = false;
        }
        if(ImGui::Button("SpriteReference")){
            actor_type = "SpriteReference";
            editing_type = false;
        }
        if(ImGui::Button("Animation")){
            actor_type = "Animation";
            editing_type = false;
        }

        ImGui::End();
    }

    if(add_actor_node_dialogue_open){
        ImGui::Begin("Add ActorNode");

        if(ImGui::Button("Actor")){
            actor_nodes_to_be_added_at_end_of_frame.push_back(std::make_unique<ActorNode>());
            actor_nodes_to_be_added_at_end_of_frame.back()->actor_type = "Actor";
            actor_nodes_to_be_added_at_end_of_frame.back()->editing_name = true;
            add_actor_node_dialogue_open = false;
        }
        if(ImGui::Button("SpriteReference")){
            actor_nodes_to_be_added_at_end_of_frame.push_back(std::make_unique<ActorNode>());
            actor_nodes_to_be_added_at_end_of_frame.back()->actor_type = "SpriteReference";
            actor_nodes_to_be_added_at_end_of_frame.back()->editing_name = true;
            add_actor_node_dialogue_open = false;
        }
        if(ImGui::Button("Animation")){
            actor_nodes_to_be_added_at_end_of_frame.push_back(std::make_unique<ActorNode>());
            actor_nodes_to_be_added_at_end_of_frame.back()->actor_type = "Animation";
            actor_nodes_to_be_added_at_end_of_frame.back()->editing_name = true;
            add_actor_node_dialogue_open = false;
        }

        ImGui::End();
    }

    if(ImGui::IsItemClicked()){
        //_program->drag_drop_stack.push_back(DragDrop{this, _parent});
    }

    if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)){
        ImGui::SetDragDropPayload("CONTENT_BROWSER_DATA", this, sizeof(this));

        ImGui::EndDragDropSource();
    }

    //This is in case file is dropped on folder
    if(ImGui::BeginDragDropTarget()){

        if(ImGui::IsMouseReleased(ImGuiMouseButton_Left)){
            Console::PrintLine("Drop");
            //_program->drag_drop_stack.back().move_to_folder = this;
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
        }
        if(ImGui::MenuItem("New Actor/Component")){
            //actor_nodes_to_be_added_at_end_of_frame.push_back(std::make_unique<ActorNode>());
            //actor_nodes_to_be_added_at_end_of_frame.back()->editing_name = true;

            add_actor_node_dialogue_open = true;
            
        }

        if(ImGui::MenuItem("Change ActorNode Type")){
            //actor_nodes_to_be_added_at_end_of_frame.push_back(std::make_unique<ActorNode>());
            //actor_nodes_to_be_added_at_end_of_frame.back()->editing_name = true;

            editing_type = true;
            
        }

        ImGui::EndPopup();
    }

    if(folder_opened){

        for(int index = 0; index < actor_nodes.size(); index++){
            actor_nodes[index]->Update(index,this,(_parent ? path+"/" : "")+name,_program);
        }

        ImGui::TreePop();
        
    }
}

void ActorNode::AddActorNodesRecursive(){
    for(auto&& i : actor_nodes_to_be_added_at_end_of_frame){
        actor_nodes.push_back(std::move(i));
    }
    actor_nodes_to_be_added_at_end_of_frame.clear();

    for(auto&& i : actor_nodes){
        i->AddActorNodesRecursive();
    }
}

void ActorNode::DeleteActorNodesMarkedForDeletion(){
    //Console::PrintLine(file_name);

    for(int i = actor_nodes.size()-1; i >= 0; i--){
        if(actor_nodes[i]->to_be_deleted) actor_nodes.erase(actor_nodes.begin() + i);
    }

    for(int i = actor_nodes.size()-1; i >= 0; i--){
        actor_nodes[i]->DeleteActorNodesMarkedForDeletion();
    }
}

JsonDictionary ActorNode::WriteToJson(){

    JsonDictionary this_actor = JsonDictionary();

    this_actor.Set("name", name);
    this_actor.Set("type", actor_type);
    this_actor.Set("children", JsonArray());

    for(const auto& child : actor_nodes){
        this_actor.Get("children").AsArray().Push(child->WriteToJson());
    }

    return this_actor;
}
    
}