#pragma once
#include <memory>
#include "../imgui/imgui.h"
#include "../imgui/misc/cpp/imgui_stdlib.h"
#include "../json/json_variant.h"
#include "vector2f.h"

namespace UFOEngineStudio{

class ProgramState;

class Property{
public:
    static inline int id_incrementation_counter = 0;

    std::string name = "Hello World";

    int id = 0;

    Property(std::string _name);
    virtual void Update(ProgramState* _program_state);

    virtual JsonDictionary AsJson();


    virtual std::unique_ptr<Property> Copy(){
    }
};

class PropertyInt : public Property{
public:
    int value = 0;

    PropertyInt(std::string _name, int _value);

    void Update(ProgramState* _program_state);

    JsonDictionary AsJson();

    std::unique_ptr<Property> Copy(){
        return std::make_unique<PropertyInt>(name, value);
    }

};

class PropertyString : public Property{
public:
    std::string text = "";

    PropertyString(std::string _name, std::string _text);

    void Update(ProgramState* _program_state);

    JsonDictionary AsJson();

    std::unique_ptr<Property> Copy(){
        return std::make_unique<PropertyString>(name, text);
    }

};

class PropertyFloat : public Property{
public:
    float value = 0;

    PropertyFloat(std::string _name, float _value);

    void Update(ProgramState* _program_state);

    JsonDictionary AsJson();

    std::unique_ptr<Property> Copy(){
        return std::make_unique<PropertyFloat>(name, value);
    }
    
};

class PropertyVector2f : public Property{
public:
    float x = 0.0f;
    float y = 0.0f;

    PropertyVector2f(std::string _name, float _x, float _y);

    void Update(ProgramState* _program_state);

    JsonDictionary AsJson();

    std::unique_ptr<Property> Copy(){
        return std::make_unique<PropertyVector2f>(name, x, y);
    }

    Vector2f AsVector2f(){
        return Vector2f(x,y);
    }
    
};

}