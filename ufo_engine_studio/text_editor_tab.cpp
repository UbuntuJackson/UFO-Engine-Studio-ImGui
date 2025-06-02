#include "../imgui/imgui.h"
#include "../imgui/misc/cpp/imgui_stdlib.h"
#include "text_editor_tab.h"
#include "../console/console.h"

namespace UFOEngineStudio{

int FilterTabs(ImGuiInputTextCallbackData* data)
{
    
    data->InsertChars(data->CursorPos,"    ");
    
    return 0;
}

bool TextEditorTab::DetermineIfEdited(){
    return text == last_saved_text;
}

void TextEditorTab::OnActive(ProgramState* _program){

    ImGui::Begin("Text- and Code Editing");
    ImGui::InputTextMultiline("##TextEditor", &text, ImVec2(ImGui::GetWindowSize()), ImGuiInputTextFlags_CallbackCompletion, FilterTabs);
    
    ImGui::End();
}

void TextEditorTab::OnSave(){
    //File will have no name if it isn't read or created with respect to file system
    if(name != ""){
        File f;
        f.Insert(text);
        f.Write(path);

        last_saved_text = text;
    }
}

};