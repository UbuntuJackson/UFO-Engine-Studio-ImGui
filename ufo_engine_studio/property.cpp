#include "../imgui/imgui.h"
#include "../imgui/misc/cpp/imgui_stdlib.h"
#include "../json/json_variant.h"
#include "property.h"
#include "program_state.h"

namespace UFOEngineStudio{

//Property

Property::Property(std::string _name){
    id = id_incrementation_counter++;
    name = _name;
}
void Property::Update(ProgramState* _program_state){
    
}

JsonDictionary Property::AsJson(){
    
}

//PropertyInt

PropertyInt::PropertyInt(std::string _name, int _value) : Property(_name), value{_value}{}

void PropertyInt::Update(ProgramState* _program_state){
    ImGui::InputInt((name+"##"+std::to_string(id)).c_str(), &value);
}

JsonDictionary PropertyInt::AsJson(){
    JsonDictionary j = JsonDictionary();
    j.Set("name", name);
    j.Set("value", value);
    j.Set("type", "int");
    return j;
}

//PropertyBool
PropertyBool::PropertyBool(std::string _name, bool _value) : Property(_name), checked{_value}{}

void PropertyBool::Update(ProgramState* _program_state){
    ImGui::Checkbox((name+"##"+std::to_string(id)).c_str(), &checked);
}

JsonDictionary PropertyBool::AsJson(){
    JsonDictionary j = JsonDictionary();
    j.Set("name", name);
    j.Set("value", checked);
    j.Set("type", "bool");
    return j;
}

//PropertyString

PropertyString::PropertyString(std::string _name, std::string _text) : Property(_name), text{_text}{}

void PropertyString::Update(ProgramState* _program_state){
    ImGui::InputText((name+"##"+std::to_string(id)).c_str(), &text);
}

JsonDictionary PropertyString::AsJson(){
    JsonDictionary j = JsonDictionary();
    j.Set("name", name);
    j.Set("value", text);
    j.Set("type", "String");
    return j;
}

//PropertyFloat

PropertyFloat::PropertyFloat(std::string _name, float _value) : Property(_name), value{_value}{}

void PropertyFloat::Update(ProgramState* _program_state){
    ImGui::InputFloat((name+"##"+std::to_string(id)).c_str(), &value);
}

JsonDictionary PropertyFloat::AsJson(){
    JsonDictionary j = JsonDictionary();
    j.Set("name", name);
    j.Set("value", value);
    j.Set("type", "float");
    return j;
}

//PropertyVector2f
PropertyVector2f::PropertyVector2f(std::string _name, float _x, float _y) : Property(_name), x{_x}, y{_y}{}

void PropertyVector2f::Update(ProgramState* _program_state){
    ImGui::InputFloat((name+".x"+"##"+std::to_string(id)).c_str(), &x);
    ImGui::InputFloat((name+".y"+"##"+std::to_string(id)).c_str(), &y);
}

JsonDictionary PropertyVector2f::AsJson(){
    JsonDictionary j = JsonDictionary();
    j.Set("name", name);
    j.Set("x", x);
    j.Set("y", y);
    j.Set("type", "Vector2f");
    return j;
}

}