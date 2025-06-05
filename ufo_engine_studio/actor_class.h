#pragma once
#include <string>
#include <vector>

struct ActorClass{
    std::string name;
    std::vector<std::string> variables;
};

struct ActorVariant{
    int class_id;
    std::string category;
};