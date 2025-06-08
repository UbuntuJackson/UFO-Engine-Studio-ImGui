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

namespace UFOEngineStudio{

struct Project{
    std::string path;
    bool is_valid;
    std::string name;

    std::map<int, ActorClass> actor_classes;
    std::vector<ActorVariant> actor_categories;
    
    void AddActorVariantFromFile(std::string _file){

    }

    void AddActorVariantFromActorClass(ActorClass _actor_class, const std::string& _category){
        for(auto&& [actor_class_id,actor_class] : actor_classes){
            if(actor_class.name == _actor_class.name){
                actor_categories.push_back(ActorVariant{actor_class_id, _category});
                return;
            }
        }

        actor_classes[actor_classes.size()] = _actor_class;
        actor_categories.push_back(ActorVariant{int(actor_classes.size()-1), _category});
    }
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

    Project project = Project{"None", false};
    bool show_new_project_dialogue = false;
    std::string pending_project_name = "";

    std::vector<std::unique_ptr<UFOEngineStudio::Tab>> tabs;
    UFOEngineStudio::Tab* active_tab = nullptr;

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

        std::sort(project.actor_categories.begin(), project.actor_categories.begin(), [](const ActorVariant& _a, const ActorVariant& _b){
            return _a.category < _b.category;
        });
    } 

    void CleanUp();

};

};