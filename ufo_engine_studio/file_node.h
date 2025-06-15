#pragma once
#include <filesystem>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include "../console/console.h"

namespace UFOEngineStudio{

class ProgramState;
class Directory;
class FileNode{
public:

    static inline int file_id_counter = 0;
    int id = 0;

    bool editing_name = false;
    void TurnOnEditMode(){
        editing_name = true;
        old_file_name = file_name;
    }
    
    bool to_be_deleted = false;
    std::vector<std::unique_ptr<FileNode>> file_nodes_to_be_added_at_end_of_frame;

    std::vector<std::unique_ptr<FileNode>> file_nodes;

    std::string file_name;
    std::string old_file_name;

    FileNode();

    //This parses a folder
    static std::unique_ptr<FileNode> ParseFolder(std::string _path);

    virtual void Update(int _file_index, Directory* _parent, std::string path , ProgramState* _program);

    void AddFileNodesRecursive();

    void DeleteFileNodesMarkedForDeletion();

    void Sort();

    void Traverse();

    void SearchForHeaderFiles(ProgramState* _program, std::string _path);

    FileNode(const FileNode&) = delete;
    //FileNode(FileNode&) = delete;

};

}