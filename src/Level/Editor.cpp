#include <Level/Editor.h>
#include <emscripten/emscripten.h>

using namespace Level;


Editor* Editor::Instance = NULL;

void editor_update_vals() {
	printf("editor_update_vals %d %d\n",
			EM_ASM_INT_V({return jsEditor_Layer.pos_x; }),
			EM_ASM_INT_V({return jsEditor_Layer.pos_y; }));

	Editor::Instance->getCurrentLevel()->Layers[0]->X = EM_ASM_INT_V({return jsEditor_Layer.pos_x; });
	Editor::Instance->getCurrentLevel()->Layers[0]->Y = EM_ASM_INT_V({return jsEditor_Layer.pos_y; });
	Editor::Instance->getCurrentLevel()->Layers[0]->updateChanges();
}


Editor::Editor() {
	_isActivated = false;
	Instance = this;
}

void Editor::toggle() {
	EM_ASM_INT({
		jsEditor_Layer.pos_x = $0;
		jsEditor_Layer.pos_y = $1;
		jsEditor_toggle();
	}, (int)_current_level->Layers[0]->X, (int)_current_level->Layers[0]->Y);

	_isActivated = !_isActivated;
}