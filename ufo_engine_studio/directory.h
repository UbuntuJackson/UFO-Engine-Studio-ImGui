#pragma once

#include "file_node.h"

namespace UFOEngineStudio{

class ProgramState;

class Directory : public FileNode{
public:
    //_file_index : the number of the file in it's local directory
    //_parent : The local directory the file or directory resides in
    //_path : the full path relative to the working directory
    //_program : the program state with it's various variables.
    void Update(int _file_index, Directory*,std::string path , ProgramState*);
    Directory() = default;

    Directory(const Directory&) = delete;
};

}