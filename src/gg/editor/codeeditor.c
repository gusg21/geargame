#include "codeeditor.h"

#include "../assets.h"

void CodeEditor_Create(gg_code_editor_t* code_editor) { TextEditor_create(&code_editor->text_editor); }

void CodeEditor_Do(gg_code_editor_t* code_editor) {
    igBegin("Code Editor", &code_editor->open, 0);
    {
        if (code_editor->text_editor_pair != NULL) {
            igText("Editing asset %s", code_editor->text_editor_pair->name);

            igSameLine2();

            if (igButton("Save Asset", (ImVec2){100, 17})) {
                CodeEditor_SaveScriptAsset(code_editor, code_editor->text_editor_pair);
            }
        } else {
            igText("Pick an asset from the Assets Viewer to edit its code");
        }

        TextEditor_render(&code_editor->text_editor);
    }
    igEnd();
}

void CodeEditor_EditScriptAsset(gg_code_editor_t* code_editor, gg_asset_pair_t* asset_pair) {
    TextEditor_setText(&code_editor->text_editor, asset_pair->asset.data.as_script.text);
    code_editor->text_editor_pair = asset_pair;
}

void CodeEditor_SaveScriptAsset(gg_code_editor_t* code_editor, gg_asset_pair_t* asset_pair) {
    if (asset_pair->asset.data.as_script.text != NULL) free(asset_pair->asset.data.as_script.text);

    char* code = TextEditor_getText(&code_editor->text_editor);  // Managed by us now
    asset_pair->asset.data.as_script.text = code;
    asset_pair->asset.data.as_script.ok = true;
}