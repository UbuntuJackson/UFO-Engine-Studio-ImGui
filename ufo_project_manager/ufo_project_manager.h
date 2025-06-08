#pragma once
#include <string>
#include <vector>
#include "../console/console.h"

class InstanceProperty{
public:
    std::string identifier;
    std::string alias;

};

class InstancePropertyInt : public InstanceProperty{
public:
    int value;
};

class InstancePropertyIntRange : public InstanceProperty{
public:
    int value;
    int lower_boundary = 0;
    int upper_boundary = 100;
};

class InstancePropertyFloat : public InstanceProperty{
public:
    float value;
};

class InstancePropertyFloatRange : public InstanceProperty{
public:
    float value;
    float lower_boundary = 0.0f;
    float upper_boundary = 100.0f;
};

class Variable{
public:
    struct Attribute{
        struct AttributeArgument{
            std::string data_type;
            std::string value;
        };

        std::string name;
        std::vector<AttributeArgument> args;
    };

    std::string data_type;
    std::string name;
    std::string default_value;

    std::vector<Attribute> attributes;

    void Print(){
        for(const auto& attribute : attributes){
            Console::Print("Attribute:",attribute.name, "= ");
            for(const auto& arg : attribute.args){
                Console::Print(arg.data_type, ":", arg.value, "\n");
            }
        }
        Console::PrintLine("Variable:",data_type, name, "=", default_value);
    }
};

class UFOProjectManager{
public:
    
    UFOProjectManager(){
        auto v = Variable();
        v.name = "interval_milliseconds";
        v.data_type = "int";
        v.default_value = "1000";
        v.attributes = {
            Variable::Attribute{
                "alias",
                {Variable::Attribute::AttributeArgument{"String", "spawn interval (milliseconds)"}}
            }
        };

        v.Print();
    }

};