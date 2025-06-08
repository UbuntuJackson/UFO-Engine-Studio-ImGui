#pragma once
#include "../vector2f.h"
#include "../property.h"
#include "../../imgui/imgui.h"
#include "../program_state.h"
#include "../im_vec.h"

namespace UFOEngineStudio{

class ActorEditorObject{
public:
    
    PropertyVector2f* local_position_handle = nullptr;
    
    //These are the variables that you can configure via the browser
    std::vector<std::unique_ptr<Property>> properties;

    //These are the variables that you can configure via the browser, except these properties are specifically to generate the constructor
    std::vector<std::unique_ptr<Property>> constructor_properties;

    

    template<typename tType, typename ...tArgs>
    tType* AddConstructorProperty(tArgs... _args){
        auto u_property = std::make_unique<tType>(_args...);
        tType* property_ptr = u_property.get();
        constructor_properties.push_back(std::move(u_property));

        return property_ptr;
    }

    template<typename tType, typename ...tArgs>
    tType* AddProperty(tArgs... _args){
        auto u_property = std::make_unique<tType>(_args...);
        tType* property_ptr = u_property.get();
        properties.push_back(std::move(u_property));

        return property_ptr;
    }

    ActorEditorObject(){
        local_position_handle = AddConstructorProperty<PropertyVector2f>("local position", 0.0f, 0.0f);
    }

    virtual void Update(ActorEditorObject* _parent_editor_object, Vector2f _parent_position){
        Vector2f global_position = _parent_position + local_position_handle->AsVector2f();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        Console::PrintLine("EditorObject", global_position.x, global_position.y);
        
        draw_list->AddRectFilled(ImAdd(ImVec2(global_position.x, global_position.y), ImGui::GetWindowPos()),
            ImAdd(ImVec2(global_position.x+16.0f, global_position.y+16.0f), ImGui::GetWindowPos()),ImColor(255,0,0,255));
    }

    void PropertyUpdate(ProgramState* _program){
        ImGui::Text("-- Constructor Arguments --");

        for(const auto& property : constructor_properties){
            property->Update(_program);
        }

        ImGui::Text("-- General Properties --");

        for(const auto& property : properties){
            property->Update(_program);
        }
    }

    virtual void NullHandles(){
        local_position_handle = nullptr;
    }

    virtual void OnSetupProperty(PropertyInt* _p){

    }

    virtual void OnSetupProperty(PropertyString* _p){
        
    }

    virtual void OnSetupProperty(PropertyFloat* _p){
        
    }

    virtual void OnSetupProperty(PropertyVector2f* _p){
        if(_p->name == "local position"){
            local_position_handle = _p;
        }
    }

    void SetPropertiesFromJson(JsonDictionary* _json){
        properties.clear();
        constructor_properties.clear();

        //This goes in editor_object
        for(const auto& property : _json->Get("constructor_properties").AsArray().Iterable()){
            std::string local_type = property->AsDictionary().Get("type").AsString();
            std::string local_name = property->AsDictionary().Get("name").AsString();

            if(local_type == "int"){
                
                int local_value = property->AsDictionary().Get("value").AsInt();
                OnSetupProperty(AddConstructorProperty<PropertyInt>(local_name, local_value));
                
            }
            else if(local_type == "String"){
                
                std::string local_value = property->AsDictionary().Get("value").AsString();
                OnSetupProperty(AddConstructorProperty<PropertyString>(local_name, local_value));

            }
            else if(local_type == "float"){

                float local_value = property->AsDictionary().Get("value").AsFloat();
                OnSetupProperty(AddConstructorProperty<PropertyFloat>(local_name, local_value));
                
            }
            else if(local_type == "Vector2f"){
                float local_value_x = property->AsDictionary().Get("x").AsFloat();
                float local_value_y = property->AsDictionary().Get("y").AsFloat();
                OnSetupProperty(AddConstructorProperty<PropertyVector2f>(local_name, local_value_x, local_value_y));
                
            }
            else{
                Console::PrintLine("ActorNode::ReadFromJson:","Unknown type", local_type);
            }

            
        }

        //This goes in editor_object
        for(const auto& property : _json->Get("properties").AsArray().Iterable()){
            std::string local_type = property->AsDictionary().Get("type").AsString();
            std::string local_name = property->AsDictionary().Get("name").AsString();

            if(local_type == "int"){
                
                int local_value = property->AsDictionary().Get("value").AsInt();
                OnSetupProperty(AddProperty<PropertyInt>(local_name, local_value));
                
            }
            else if(local_type == "String"){
                
                std::string local_value = property->AsDictionary().Get("value").AsString();
                OnSetupProperty(AddProperty<PropertyString>(local_name, local_value));

            }
            else if(local_type == "float"){

                float local_value = property->AsDictionary().Get("value").AsFloat();
                OnSetupProperty(AddProperty<PropertyFloat>(local_name, local_value));
                
            }
            else if(local_type == "Vector2f"){
                float local_value_x = property->AsDictionary().Get("x").AsFloat();
                float local_value_y = property->AsDictionary().Get("y").AsFloat();
                OnSetupProperty(AddProperty<PropertyVector2f>(local_name, local_value_x, local_value_y));
                
            }
            else{
                Console::PrintLine("ActorNode::ReadFromJson:","Unknown type", local_type);
            }
            
        }
    }

};

}