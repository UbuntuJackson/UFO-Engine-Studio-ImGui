#pragma once
#include <string>
#include "../console/console.h"
#include <SDL3/SDL.h>
#include "../json/json_variant.h"
#include "tab.h"
#include "file_node.h"
#include "actor_class.h"
#include "actor_node.h"
#include <memory>
#include <algorithm>
#include <filesystem>
#include "../json/json_variant.h"
#include <iostream>

namespace UFOEngineStudio{

struct Project{
    
    bool is_valid;
    std::string name;
    
    std::vector<ActorClass> actor_classes;

};

struct DragDrop{
    FileNode* file_to_be_moved = nullptr;
    FileNode* owner = nullptr;
    std::string owner_path;
    FileNode* move_to_folder = nullptr;
    std::string move_to_path;
    int index_in_move_to_folder = 0;
};

class ProgramState{
public:

    SDL_Window* window = nullptr;

    Project project = Project{false};
    bool show_new_project_dialogue = false;
    std::string pending_project_name = "";

    std::vector<std::unique_ptr<UFOEngineStudio::Tab>> tabs;
    UFOEngineStudio::Tab* active_tab = nullptr;

    bool should_refresh_properties_on_all_nodes = false;

    bool should_refresh_working_directory = false;

    std::string working_directory_path;
    std::unique_ptr<FileNode> opened_directory;

    //The contents that are being drag dropped. I made this a vector for now, but ideally it should just be a pointer, coupled
    // with a vector of all the contents that are being moved. When you think about it, conventionally you can select multiple
    // files, but you can only drop the in one location. Thus, a pointer to location, and a vector with the contents.
    //Also the ImGui payload is unused since I resorted to implementing drag and drop in a bit of an unconventional manner.
    std::vector<DragDrop> drag_drop_stack;

    SDL_Texture* example_texture;

    ProgramState(SDL_Renderer* _renderer);

    //This is meant to write the project file for the UFO-Engine project you are making
    void WriteProjectFile(std::string _path);

    //This is meant to load the project file for the UFO-Engine project you are making
    void OpenProjectFile(std::string _path);

    //This opens a folder but will also be used when opening a project
    void OpenFolder(std::string _path);

    bool should_search_working_directory_for_exposed_actor_classes = true;

    void SearchWorkingDirectoryForExposedActorClasses(){
        Console::PrintLine("Refreshing working directory:", working_directory_path);
        opened_directory->SearchForHeaderFiles(this, "");
    }

    void ImportHeaderFileToProject(std::string _path){

        int result = system((std::string("cd .. && cd header_tool && python3 project.py ")+_path).c_str());
        if(result) Console::PrintLine("ProgramState::ImportHeaderFileToProject() Result returned true");
        
        std::string exported_variables_supposed_location = _path + "_exported_variables.json";

        if(!File::Exists(exported_variables_supposed_location)) return;

        JsonDictionary d = JsonVariant::Read(exported_variables_supposed_location);

        std::string class_name = "";
        std::vector<std::unique_ptr<Property>> exported_properties;
        for(const auto& variable : d.Get("exported_variables").AsArray().Iterable()){
            
            std::string local_name = variable->AsDictionary().Get("name").AsString();
            std::string data_type = variable->AsDictionary().Get("data_type").AsString();
            std::string default_value = variable->AsDictionary().Get("default_value").AsString();

            if(data_type == "class"){
                if(class_name.empty()) class_name = local_name;
                else Console::PrintLine("[x] UFO-Engine Studio: Error, two classes found in", _path);
            }
            else{
                try{
                    if(data_type == "std::string"){
                        std::string text = default_value;
                        auto p = std::make_unique<PropertyString>(local_name, text);
                        exported_properties.push_back(std::move(p));
                    }
                    if(data_type == "bool"){
                        bool checked = (default_value == "true");
                        auto p = std::make_unique<PropertyBool>(local_name, checked);
                        exported_properties.push_back(std::move(p));
                    }
                    if(data_type == "int"){
                        int value = std::stoi(default_value);
                        auto p = std::make_unique<PropertyInt>(local_name, value);
                        exported_properties.push_back(std::move(p));
                    }
                    if(data_type == "float"){
                        bool value = std::stof(default_value);
                        auto p = std::make_unique<PropertyFloat>(local_name, value);
                        exported_properties.push_back(std::move(p));
                    }
                }
                catch(const std::invalid_argument& err){
                    Console::PrintLine("[x] UFO-Engine Studio: Error, in ProgramState::ImportHeaderFileToProject()\n",
                        "    Some sort of convetion error happened with argument '"+default_value+"' with function call" ,err.what(),
                        "\n    in file:", _path);
                }
            }

        }

        //Produce the generated.h and sh&t

        bool class_already_added = false;

        for(int actor_class_index = project.actor_classes.size()-1; actor_class_index >= 0; actor_class_index--){
            if(project.actor_classes[actor_class_index].name == class_name){
                project.actor_classes.erase(project.actor_classes.begin()+actor_class_index);
            }
        }

        project.actor_classes.push_back(ActorClass{class_name, _path, true});

        for(auto&& p : exported_properties){
            
            project.actor_classes.back().exported_properties.push_back(std::move(p));
        }

        //Need to reload the working directory.
        should_refresh_working_directory = true;
        should_refresh_properties_on_all_nodes = true;
    }

    //This runs after the directory tree is updated
    void Update(){

        //This list will be the new drag_drop_stack.
        //All items that begin a dragdrop but aren't moved to a new folder end up here.
        std::vector<DragDrop> preserved;

        //This might seem unnecessary but it's to preserve indexes to avoid removing elements while iterating
        struct DragDropAndEraseIndex{
            DragDrop drag_drop;
            int erase_index;
            std::unique_ptr<FileNode> u_file_node_to_move;
        };

        std::vector<DragDropAndEraseIndex> to_be_moved;

        for(auto&& drag_drop : drag_drop_stack){
            int index = 0;

            //If it's the highest folder which does not have an owner.
            if(drag_drop.owner == nullptr) continue;

            for(auto&& u_ptr : drag_drop.owner->file_nodes){

                if(u_ptr.get() == drag_drop.file_to_be_moved){
                    if(drag_drop.move_to_folder != nullptr){

                        //Here the unique_ptr is moved to establish it in the directory tree
                        to_be_moved.push_back(DragDropAndEraseIndex{drag_drop, index, std::move(u_ptr)});
                    }
                    else{
                        preserved.push_back(drag_drop);
                    }
                }
                
                index++;
            }
        }

        for(auto&& item : to_be_moved){

            Console::PrintLine(item.drag_drop.owner_path, item.drag_drop.move_to_path+"/"+item.u_file_node_to_move->file_name);

            Console::PrintLine(item.drag_drop.owner_path+"/"+item.drag_drop.file_to_be_moved->file_name,
                item.drag_drop.move_to_path+"/"+item.drag_drop.file_to_be_moved->file_name);

            std::filesystem::rename(item.drag_drop.owner_path+"/"+item.drag_drop.file_to_be_moved->file_name,
                item.drag_drop.move_to_path+"/"+item.drag_drop.file_to_be_moved->file_name);

            item.drag_drop.owner->file_nodes.erase(item.drag_drop.owner->file_nodes.begin() +item.erase_index);
            item.drag_drop.move_to_folder->file_nodes.insert(item.drag_drop.move_to_folder->file_nodes.begin() + item.drag_drop.index_in_move_to_folder, std::move(item.u_file_node_to_move));
        }

        drag_drop_stack.clear();
        to_be_moved.clear();

        drag_drop_stack = preserved;
    
        //Delete file nodes marked for deletion
        if(opened_directory.get() != nullptr){

            //opened_directory->Traverse();

            opened_directory->DeleteFileNodesMarkedForDeletion();
        
            opened_directory->AddFileNodesRecursive();

            opened_directory->Sort();
        }

        if(should_refresh_working_directory){
            OpenFolder(working_directory_path);
        }
        should_refresh_working_directory = false;

        should_refresh_properties_on_all_nodes = false;

        if(should_search_working_directory_for_exposed_actor_classes) SearchWorkingDirectoryForExposedActorClasses();

        should_search_working_directory_for_exposed_actor_classes = false;

        /*std::sort(project.actor_categories.begin(), project.actor_categories.begin(), [](const ActorVariant& _a, const ActorVariant& _b){
            return _a.category < _b.category;
        });*/
    } 

    void CleanUp();

};

};