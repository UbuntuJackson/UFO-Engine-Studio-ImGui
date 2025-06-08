#pragma once

#include <memory>
#include "../vector2f.h"
#include "../property.h"
#include "editor_object.h"

namespace UFOEngineStudio{

class SpriteReferenceEditorObject : public ActorEditorObject{
public:
    PropertyString* key = nullptr;
    PropertyVector2f* offset = nullptr;
    PropertyVector2f* frame_size = nullptr;
    PropertyVector2f* scale = nullptr;
    PropertyFloat* rotation = nullptr;

    SpriteReferenceEditorObject(){
        constructor_properties.clear();
        
        key = AddConstructorProperty<PropertyString>("key", "");
        local_position_handle = AddConstructorProperty<PropertyVector2f>("local position", 0.0f, 0.0f);
        offset = AddConstructorProperty<PropertyVector2f>("offset", 0.0f, 0.0f);
        frame_size = AddConstructorProperty<PropertyVector2f>("scale", 0.0f, 0.0f);
        scale = AddConstructorProperty<PropertyVector2f>("frame size", 0.0f, 0.0f);
        rotation = AddConstructorProperty<PropertyFloat>("rotation", 0.0f);
    }

    void OnSetupProperty(PropertyFloat *_p){
        ActorEditorObject::OnSetupProperty(_p);

        if(_p->name == "rotation"){
            rotation = _p;
        }

    }

    void OnSetupProperty(PropertyString* _p){

        ActorEditorObject::OnSetupProperty(_p);

        if(_p->name == "key"){
            key = _p;
        }
    }

    void OnSetupProperty(PropertyVector2f* _p){
        ActorEditorObject::OnSetupProperty(_p);

        if(_p->name == "offset"){
            offset = _p;
        }
        if(_p->name == "scale"){
            scale = _p;
        }
        if(_p->name == "frame size"){
            frame_size = _p;
        }
    }

    void Update(ActorEditorObject* _parent_editor_object, Vector2f _parent_position){
        ActorEditorObject::Update(_parent_editor_object, _parent_position);


    }

};

}