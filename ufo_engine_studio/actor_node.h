#pragma once
#include <string>
#include <vector>
#include <memory>
#include "property.h"
#include "vector2f.h"
#include "editor_objects/editor_object.h"

class JsonDictionary;

namespace UFOEngineStudio{

class ProgramState;
class ActorComposerTab;

class ActorNode{
public:
    bool to_be_deleted = false;
    bool editing_name = false;
    bool editing_type = false;
    bool show_properties = false;
    int id = 0;
    inline static int id_incrementation_counter = 0;

    bool add_actor_node_dialogue_open = false;

    std::unique_ptr<ActorEditorObject> editor_object;

    std::string name = "";
    std::string actor_type = "Actor";

    PropertyVector2f* local_position = nullptr;

    void DrawInLevel(ActorNode* _parent, Vector2f _parent_global_position){
        editor_object->Update((_parent ? _parent->editor_object.get() : nullptr), _parent_global_position);
        
        for(const auto& actor_node : actor_nodes){
            actor_node->DrawInLevel(this, _parent_global_position + editor_object->local_position_handle->AsVector2f());
        }
    }

    ActorNode();

    std::vector<std::unique_ptr<ActorNode>> actor_nodes;
    std::vector<std::unique_ptr<ActorNode>> actor_nodes_to_be_added_at_end_of_frame;

    void Update(int _file_index, ActorComposerTab* _actor_composer, ActorNode* _parent, std::string path , ProgramState* _program);

    void AddActorNodesRecursive();

    void DeleteActorNodesMarkedForDeletion();

    void Sort();

    JsonDictionary WriteToJson();

    //static std::unique_ptr<ActorNode> ReadFromJsonTypeDiverse(ProgramState* _program_state, JsonDictionary* _json);
    //static void SetAttributesFromJsonTypeDiverse(JsonDictionary* _json, ActorNode* _instance);

    std::unique_ptr<ActorEditorObject> CreateEditorObjectWithTypeFromString(std::string _actor_type);

    void ReadFromJson(JsonDictionary* _json);
    
};

}