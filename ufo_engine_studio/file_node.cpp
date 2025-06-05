#include <filesystem>
#include <memory>
#include <vector>
#include <string>
#include "../console/console.h"
#include "directory.h"
#include "file.h"
#include "file_node.h"
#include "program_state.h"
#include "../imgui/imgui.h"
#include "../imgui/misc/cpp/imgui_stdlib.h"

namespace UFOEngineStudio{

FileNode::FileNode(){
    id = file_id_counter;
    file_id_counter++;
}

std::unique_ptr<FileNode> FileNode::ParseFolder(std::string _path){
    std::unique_ptr<Directory> directory = std::make_unique<Directory>();

    directory->file_name = _path.substr(_path.find_last_of("/")+1);

    for(const auto& directory_entry : std::filesystem::directory_iterator{_path}){

        std::error_code ec;
        std::string s_path = std::string(directory_entry.path().string());
        if(std::filesystem::is_directory(directory_entry,ec)){

            directory->file_nodes.push_back(FileNode::ParseFolder(s_path));

        }
        else{

            std::string file_name = s_path.substr(s_path.find_last_of("/")+1);

            auto file = std::make_unique<TreeFile>();

            file->file_name = file_name;

            directory->file_nodes.push_back(std::move(file));

        }
    }

    return std::move(directory);
}

void FileNode::Update(int _file_index, Directory* _parent,std::string path , ProgramState* _program){
    
}

void FileNode::AddFileNodesRecursive(){
    for(auto&& i : file_nodes_to_be_added_at_end_of_frame){
        file_nodes.push_back(std::move(i));
    }
    file_nodes_to_be_added_at_end_of_frame.clear();

    for(auto&& i : file_nodes){
        i->AddFileNodesRecursive();
    }
}

void FileNode::DeleteFileNodesMarkedForDeletion(){
    //Console::PrintLine(file_name);

    for(int i = file_nodes.size()-1; i >= 0; i--){
        if(file_nodes[i]->to_be_deleted) file_nodes.erase(file_nodes.begin() + i);
    }

    for(int i = file_nodes.size()-1; i >= 0; i--){
        file_nodes[i]->DeleteFileNodesMarkedForDeletion();
    }
}

void FileNode::Sort(){
    //return;
    
    auto comp = [](const std::unique_ptr<FileNode>& _a, const std::unique_ptr<FileNode>& _b){

        return _a->file_name < _b->file_name;

        /*bool char_same = false;

        if(int(_a->file_name.size()) > int(_b->file_name.size())){

            for(int i = 0; i < int(_b->file_name.size()); i++){
                if(int(_a->file_name[i]) == int(_b->file_name[i])){
                    char_same = true;
                }
                else{
                    return int(_a->file_name[i]) < int(_b->file_name[i]);
                }
            }
           
        }
        else{
            for(int i = 0; i < int(_a->file_name.size()); i++){
                if(int(_a->file_name[i]) == int(_b->file_name[i])){
                    char_same = true;
                }
                else{
                    return int(_a->file_name[i]) < int(_b->file_name[i]);
                }
            }
            
        }

        if(char_same) return _a->file_name.size() > _b->file_name.size();

        return false;*/
    };

    std::sort(file_nodes.begin(), file_nodes.end(),comp);

    for(auto&& i : file_nodes){
        i->Sort();
    }
}

void FileNode::Traverse(){
    Console::PrintLine("Name:", file_name,"Size:", file_nodes.size());
    //Console::PrintLine("It:", file_nodes.end()->get());

    for(auto iterator = file_nodes.begin(); iterator != file_nodes.end(); ++iterator){
        (*iterator)->Traverse();
    }
}

}