#include <memory>
#include <Main.h>
#include <level.h>
#include <console.h>
#include "ufo_engine_studio/editor.h"

int main(){

    auto main = ufo::Main(1600, 900);

    auto e = std::make_unique<ufo::Engine>();

    e->level = std::move(std::make_unique<UFOEngineStudio::Editor>());
    e->engine_path = "../UFO-Engine-GL";

    main.StartWithImGui(std::move(e));

    return 0x0;
}