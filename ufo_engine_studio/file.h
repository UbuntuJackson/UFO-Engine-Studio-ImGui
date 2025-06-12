#pragma once
#include <string>
#include "file_node.h"

namespace UFOEngineStudio{

class ProgramState;

class TreeFile : public FileNode{
public:
    bool is_new_file = false;
    std::string path_for_drag_drop_payload_use_only = "";

    TreeFile(bool _is_new_file = false) : is_new_file{_is_new_file}{
        
    }

    void Update(int _file_index, Directory* _parent,std::string path , ProgramState* _program);

    TreeFile(const TreeFile&) = delete;

};

}