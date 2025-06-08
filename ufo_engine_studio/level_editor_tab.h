#pragma once
#include <memory>
#include "tab.h"
#include "actor_node.h"

namespace UFOEngineStudio{

class ProgramState;

class LevelEditorTab : public Tab{
public:
    std::unique_ptr<ActorNode> level_root;

    bool actor_variant_config_open = false;

    LevelEditorTab(ProgramState* _program_state, std::string _file);

    void OnActive(ImGuiID _local_dockspace_id , ProgramState* _program_state);

    void OnMakeDockSpace(ImGuiID _local_dockspace_id, ProgramState* _program_state);
};

}