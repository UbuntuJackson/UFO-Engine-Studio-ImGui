#pragma once
#include <string>
#include <vector>
#include <memory>
#include "property.h"

struct ActorClass{
    std::string name;
    std::string header_file = "";
};

struct ActorVariant{
    int class_id;
    std::string category;
};