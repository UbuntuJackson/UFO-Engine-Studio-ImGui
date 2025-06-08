#include <string>
#include "../console/console.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "../json/json_variant.h"
#include "level_editor_tab.h"
#include <memory>
#include "program_state.h"
#include "text_editor_tab.h"
#include "actor_class.h"
#include "property.h"

namespace UFOEngineStudio{

ProgramState::ProgramState(SDL_Renderer* _renderer){
    SDL_Surface* example_surface = IMG_Load("../res/placeholder_icon.png");
    assert(example_surface != nullptr);
    example_texture = SDL_CreateTextureFromSurface(_renderer ,example_surface);
    assert(example_texture != nullptr);
    SDL_DestroySurface(example_surface);

    tabs.push_back(std::make_unique<UFOEngineStudio::LevelEditorTab>(this,"LevelTab1"));

    project.AddActorVariantFromActorClass(ActorClass{"Actor", "<actor.h>"}, "Engine");
    project.AddActorVariantFromActorClass(ActorClass{"Animation", "<animation.h>"}, "Engine");
    project.AddActorVariantFromActorClass(ActorClass{"SpriteReference", "<sprite_reference.h>"}, "Engine");

    project.AddActorVariantFromActorClass(ActorClass{"Pingu"}, "Actors");
    project.AddActorVariantFromActorClass(ActorClass{"Pingu"}, "Actors");
    project.AddActorVariantFromActorClass(ActorClass{"Pingu"}, "Actors");
    project.AddActorVariantFromActorClass(ActorClass{"Pingu"}, "Actors");
    project.AddActorVariantFromActorClass(ActorClass{"Pingu"}, "Actors");
    project.AddActorVariantFromActorClass(ActorClass{"Pingu"}, "Actors");
    project.AddActorVariantFromActorClass(ActorClass{"Pingu"}, "Actors");
    project.AddActorVariantFromActorClass(ActorClass{"Pingu"}, "Actors");
    project.AddActorVariantFromActorClass(ActorClass{"Pingu"}, "Actors");
    project.AddActorVariantFromActorClass(ActorClass{"Pingu"}, "Actors");
    project.AddActorVariantFromActorClass(ActorClass{"Pingu"}, "Actors");
    project.AddActorVariantFromActorClass(ActorClass{"Pingu"}, "Actors");
    project.AddActorVariantFromActorClass(ActorClass{"Pingu"}, "Actors");
    project.AddActorVariantFromActorClass(ActorClass{"Pingu"}, "Actors");
    project.AddActorVariantFromActorClass(ActorClass{"Spawner"}, "Actors");
    project.AddActorVariantFromActorClass(ActorClass{"Spawner"}, "Actors");
    project.AddActorVariantFromActorClass(ActorClass{"AntiSpawner"}, "Misc");
    project.AddActorVariantFromActorClass(ActorClass{"AntiSpawner"}, "Misc");
    project.AddActorVariantFromActorClass(ActorClass{"DraggableObject"}, "TestActors");
}

void ProgramState::WriteProjectFile(std::string _path){
    Console::PrintLine("Write project file:", _path);

    JsonDictionary project_json;
    project_json.Set("project_name", project.name);
    project_json.Set("actor_classes", JsonArray());
    project_json.Set("actor_categories", JsonArray());

    project_json.Write(_path);

    //Making sure the project is loaded.
    //OpenProjectFile(_path);
}

void ProgramState::OpenProjectFile(std::string _path){
    Console::PrintLine("Open project file:", _path);

    JsonDictionary project_json = JsonVariant::Read(_path);

    project = Project{_path, true, project_json.Get("project_name").AsString()};

    auto& actor_classes = project_json.Get("actor_classes").AsArray().Iterable();

    for(auto&& actor_class : actor_classes){
        std::string actor_class_name = actor_class->AsDictionary().Get("name").AsString();
        Console::PrintLine("Loading actor class:", actor_class_name);
        //project.actor_categories.push_back(ActorClass{actor_class_name});
    }

}

void ProgramState::OpenFolder(std::string _path){
    opened_directory = FileNode::ParseFolder(_path);
    opened_directory->file_name = "";
    working_directory_path = _path;
}

void
ProgramState::CleanUp(){
    SDL_DestroyTexture(example_texture);
}

}