#pragma once
#include "../ufo_engine_studio/tab.h"
#include "../ufo_engine_studio/editor.h"
#include <engine.h>

namespace UFOEngineStudio{

class LevelEditorTab : public Tab{
public:
    ufo::Engine* engine = nullptr;

    LevelEditorTab(ufo::Engine* _engine) : engine{_engine} {

    }

    void OnActive(ImGuiID _local_dockspace_id , Editor* _program_state){
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar;

        ImGui::Begin("Level Grid", nullptr, window_flags);

        ImGui::BeginChild("Drawing Surface",ImVec2(2000.0f, 1000.0f));

        /*auto image_buffer = new uint8_t[engine->width*engine->height*4];
        glReadPixels(0,0,engine->width, engine->height, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);

        if(image_buffer){
            ImGui::Image((void*)image_buffer, ImVec2(engine->width,engine->height));
        }

        delete[] image_buffer;*/

        ImGui::EndChild();

        ImGui::End();
    }
};

}