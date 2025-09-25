#pragma once
#include <memory>
#include "tab.h"
#include "actor_node.h"

namespace UFOEngineStudio{

class ProgramState;

class LevelEditorTab : public Tab{
public:
    std::unique_ptr<ActorNode> actor;

    bool actor_variant_config_open = false;
    bool something_was_selected_this_frame = false;

    LevelEditorTab(ProgramState* _program_state, std::string _file);

    void ConvertActorTreeToCPP(ProgramState* _program_state);

    void ConvertJsonToCPP(std::string& _handle_identifiers, std::string& _handle_struct, std::string& _header_file, std::vector<std::string>& _used_actor_classes, JsonDictionary* _json, ProgramState* _program_state);

    void Update(ProgramState* _program_state);

    void OnActive(ImGuiID _local_dockspace_id , ProgramState* _program_state);

    void OnMakeDockSpace(ImGuiID _local_dockspace_id, ProgramState* _program_state);

    void OnSave(ProgramState* _program_state);
};

}