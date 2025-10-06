#pragma once
#include "../ufo_engine_studio/tab.h"
#include "../ufo_engine_studio/editor.h"
#include <engine.h>
#include <sprite_renderer.h>
#include <level.h>
#include "../ufo_engine_studio/controllable_camera.h"
#include "../UFO-Engine-GL/imgui/imgui_internal.h"
#include "../UFO-Engine-GL/imgui/imgui.h"

namespace UFOEngineStudio{

class LevelEditorTab : public Tab{
public:
    ufo::Engine* engine = nullptr;
    Level* this_level = nullptr;

    LevelEditorTab(ufo::Engine* _engine, Editor* _editor) : Tab(), engine{_engine}{
        this_level = _editor->AddActor<Level>();
        this_level->Load();
        this_level->AddActor<ControllableCamera>(Vector2f(500.0f, 200.0f));
    }

    void OnActive(ImGuiID _local_dockspace_id , Editor* _editor, float _delta_time){


        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar;

        //ImGui::DockSpace(_local_dockspace_id);

        ImGui::Begin("ActorTree###");

        

        ImGui::End();

        ImGui::Begin(name_and_imgui_id.c_str(), nullptr, window_flags);

        //Console::PrintLine("OnActive");
        LevelUpdatePhase(_delta_time);
        LevelDrawPhase(engine->graphics.get());

        ImGui::GetWindowDrawList()->AddImage(
            (void*)(intptr_t)(dynamic_cast<ufo::OpenGLv4_5_Graphics*>(engine->graphics.get())->texture_id),
            ImVec2(0, 0),
            ImVec2(engine->width, engine->height),
            ImVec2(0,0),
            ImVec2(1,-1)
        );

        ImGui::End();
    }

    void OnMakeDockSpace(ImGuiID _local_dockspace_id, Editor* _program_state){
        ImGuiDockSpaceFill(_local_dockspace_id, ImGui::GetWindowSize(), name_and_imgui_id.c_str());
    }

    void LevelUpdatePhase(float _delta_time){
        this_level->UpdatePhrase(_delta_time);
    }

    void LevelDrawPhase(ufo::Graphics* _graphics){
        _graphics->BindFrameBuffer();
        
        glViewport(0, 0, engine->width, engine->height);
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        this_level->DrawPhase(_graphics);

        _graphics->UnbindFrameBuffer();
    }
};

}