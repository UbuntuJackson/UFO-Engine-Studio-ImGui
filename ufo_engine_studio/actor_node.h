#pragma once
#include <string>
#include <vector>
#include <memory>

class JsonDictionary;

namespace UFOEngineStudio{

class ProgramState;

class ActorNode{
public:
    bool to_be_deleted = false;
    bool editing_name = false;
    bool editing_type = false;
    int id = 0;
    inline static int id_incrementation_counter = 0;

    bool add_actor_node_dialogue_open = false;

    std::string name = "";
    std::string actor_type = "";

    ActorNode();

    std::vector<std::unique_ptr<ActorNode>> actor_nodes;
    std::vector<std::unique_ptr<ActorNode>> actor_nodes_to_be_added_at_end_of_frame;

    //This parses a folder
    static std::unique_ptr<ActorNode> ParseFolder(std::string _path);

    void Update(int _file_index, ActorNode* _parent, std::string path , ProgramState* _program);

    void AddActorNodesRecursive();

    void DeleteActorNodesMarkedForDeletion();

    void Sort();

    JsonDictionary WriteToJson();
};

}