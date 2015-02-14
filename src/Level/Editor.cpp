#include <Level/Editor.h>
#include <emscripten/emscripten.h>
#include <OpenGL/Global.h>
#include <Common/LiveLog/Builder.h>
#include <Common/LiveLog/CommonReflections.h>
#include <Storage/Geometry.h>
#include <Control/Mouse.h>

using namespace Level;

Editor* Editor::Instance = NULL;

#define UPDATE_TYPE_POS 0
#define UPDATE_TYPE_COLOR 1
#define UPDATE_TYPE_CAM 2
#define UPDATE_TYPE_REM_LAYER 3
#define UPDATE_TYPE_NEW_LAYER 4

void editor_update_vals(int type) {
	switch(type) {
        case UPDATE_TYPE_POS: {
            Editor::Instance->getCurrentLayer()->X = EM_ASM_INT_V({return editor_ui_instance.layer.x; });
            Editor::Instance->getCurrentLayer()->Y = EM_ASM_INT_V({return editor_ui_instance.layer.y; });
            Editor::Instance->getCurrentLayer()->Z = EM_ASM_INT_V({return editor_ui_instance.layer.z; });
            Editor::Instance->getCurrentLayer()->Width = EM_ASM_INT_V({return editor_ui_instance.layer.width; });
            Editor::Instance->getCurrentLayer()->Height = EM_ASM_INT_V({return editor_ui_instance.layer.height; });
            Editor::Instance->updatePositions();
            break;
        };
        case UPDATE_TYPE_COLOR: {
            int dec = EM_ASM_INT_V({return editor_ui_instance.layer.get_color_int(); });
            unsigned char* rgb = (unsigned char*)&dec;
            Editor::Instance->getCurrentLayer()->Color = glm::vec4(rgb[2] / 255.0f, rgb[1] / 255.0f, rgb[0]/255.0f, 1.0f);
            break;
        };
        case UPDATE_TYPE_CAM: {
            OpenGL::Global::g_pCamera->X = EM_ASM_INT_V({return editor_ui_instance.camera.x; });
            OpenGL::Global::g_pCamera->Y = EM_ASM_INT_V({return editor_ui_instance.camera.y; });
            break;
        };
        case UPDATE_TYPE_REM_LAYER: {
            bool deleted = false;
            for(auto iter = Editor::Instance->getCurrentLevel()->Layers.begin();
                iter != Editor::Instance->getCurrentLevel()->Layers.end(); iter++)
            {
                if((*iter) == Editor::Instance->getCurrentLayer()) {
                    Editor::Instance->getCurrentLevel()->Layers.erase(iter);
                    delete Editor::Instance->getCurrentLayer()->Renderer;
                    delete Editor::Instance->getCurrentLayer();
                    Editor::Instance->setCurrentLayer(NULL);
                    deleted = true;
                    break;
                }
            }
            if(deleted) break;

            Common::LiveLog::Builder builder(LOG_EDITOR);
            builder.setMessage("Tried to delete a layer that does not exist.");
            builder.push();
            break;
        };
        case UPDATE_TYPE_NEW_LAYER: {
            auto layer = new Model::Layer();
            layer->X = 0;
            layer->Y = 0;
            layer->Z = 1;
            layer->Width = 100;
            layer->Height = 100;
            layer->Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            layer->Renderer = new Level::Renderer::Layer(layer);
            Editor::Instance->getCurrentLevel()->Layers.push_back(layer);
            Editor::Instance->setCurrentLayer(layer);
            Editor::Instance->updateJsPositions();
        };
    }
    Editor::Instance->getCurrentLayer()->updateChanges();
}

// ------------------------------------------------------------------

Editor::Editor() {
    _buffer = OpenGL::Global::g_pLayerRenderSystem->requestCombined(4*4, 6*4);
    initial_buffer();

	_isActivated = false;
	Instance = this;
}

// ------------------------------------------------------------------

void Editor::setCurrentLevel(Model::Base *lvl) {
    _current_level = lvl;
    _current_layer = lvl->Layers[0];
}

void Editor::setCurrentLayer(Model::Layer *layer) {
    _current_layer = layer;
    if(_current_layer != NULL) updatePositions();
}

// ------------------------------------------------------------------


void Editor::onClick() {
    if(!_isActivated) return;

    int mx = Control::Mouse::X + OpenGL::Global::g_pCamera->X;
    int my = Control::Mouse::Y - OpenGL::Global::g_pCamera->Y;

    for(auto iter = _current_level->Layers.begin(); iter != _current_level->Layers.end(); iter++) {
        auto ptr = (*iter);

        if(ptr->X <= mx && mx <= (ptr->X + ptr->Width)) {
            if(ptr->Y <= my && my <= (ptr->Y + ptr->Height)) {

                _current_layer = ptr;
                updatePositions();
                updateJsPositions();
                return;
            }
        }
    }
    _current_layer = NULL;
}

void Editor::onDrag(int x, int y, int state, int mods) {
    if(!_isActivated) return;

    int difX = Control::Mouse::X - x;
    int difY = Control::Mouse::Y - y;

    if(mods & GLFW_MOD_CONTROL) {
        OpenGL::Global::g_pCamera->X -= difX;
        OpenGL::Global::g_pCamera->Y += difY;

        if(OpenGL::Global::g_pCamera->X > INT_MAX) {
            OpenGL::Global::g_pCamera->X = 0;
        }
        if(OpenGL::Global::g_pCamera->Y > INT_MAX) {
            OpenGL::Global::g_pCamera->Y = 0;
        }
    } else {
        if(state == CONTROL_MOUSE_DRAG_BEGIN) onClick();
        if(_current_layer == NULL) return;

        _current_layer->X += difX;
        _current_layer->Y += difY;
        _current_layer->updateChanges();
        updatePositions();
    }
}

// ------------------------------------------------------------------

void Editor::updateJsPositions() {
    glm::vec4 color  = (glm::vec4)_current_layer->Color;
    int dec_color = ((int)(color.r * 255.0f) << 16) + ((int)(color.g * 255.0f) << 8) + (int)(color.b * 255.0f);

    EM_ASM_INT({
        editor_ui_instance.layer.x = $0;
        editor_ui_instance.layer.y = $1;
        editor_ui_instance.layer.width = $2;
        editor_ui_instance.layer.height = $3;
        editor_ui_instance.layer.set_color_int($4);
        editor_ui_instance.camera.x = $5;
        editor_ui_instance.camera.y = $6;
        editor_ui_instance.refreshLayer();
    }, (int)_current_layer->X, (int)_current_layer->Y,
            (int)_current_layer->Width, (int)_current_layer->Height, dec_color,
            (int) OpenGL::Global::g_pCamera->X, (int) OpenGL::Global::g_pCamera->Y);
}

void Editor::toggle() {
    glm::vec4 color  = (glm::vec4)_current_layer->Color;
    int dec_color = ((int)(color.r * 255.0f) << 16) + ((int)(color.g * 255.0f) << 8) + (int)(color.b * 255.0f);

    updateJsPositions();
	EM_ASM({
        editor_ui_instance.setMode(1);
        editor_ui_instance.toggle();
	});

	_isActivated = !_isActivated;

    if(_isActivated) updatePositions();
}

void Editor::render() {
    if(_isActivated && _current_layer != NULL) {
        OpenGL::Global::g_pLayerRenderSystem->enqueueRenderJob(_buffer.index, 24);
    }
}

// ------------------------------------------------------------------

// black with some transparency
#define SELECTION_COLOR 0.86f, 0.21f, 1.0f
#define SELECTION_ALPHA 1.0f

void Editor::initial_buffer() {
    // Indices
    OpenGL::Global::g_pIndexBuffer->beginUpdate(_buffer.index, 0);
    int offset = 0;
    for(int i = 0; i < 4; i++) {
        *OpenGL::Global::g_pIndexBuffer->next() = 0 + _buffer.vindex + offset;
        *OpenGL::Global::g_pIndexBuffer->next() = 1 + _buffer.vindex + offset;
        *OpenGL::Global::g_pIndexBuffer->next() = 2 + _buffer.vindex + offset;
        *OpenGL::Global::g_pIndexBuffer->next() = 2 + _buffer.vindex + offset;
        *OpenGL::Global::g_pIndexBuffer->next() = 3 + _buffer.vindex + offset;
        *OpenGL::Global::g_pIndexBuffer->next() = 0 + _buffer.vindex + offset;
        offset += 4;
    }
    OpenGL::Global::g_pIndexBuffer->endUpdate();

    // Colors
    OpenGL::Global::g_pColorBuffer->beginUpdate(_buffer.vindex, 0);
    for(int i = 0; i < 4; i++) {
        glm::vec4* color = OpenGL::Global::g_pColorBuffer->next();
        *color = glm::vec4(SELECTION_COLOR, SELECTION_ALPHA);

        color = OpenGL::Global::g_pColorBuffer->next();
        *color = glm::vec4(SELECTION_COLOR, SELECTION_ALPHA);

        color = OpenGL::Global::g_pColorBuffer->next();
        *color = glm::vec4(SELECTION_COLOR, SELECTION_ALPHA);

        color = OpenGL::Global::g_pColorBuffer->next();
        *color = glm::vec4(SELECTION_COLOR, SELECTION_ALPHA);
    }
    OpenGL::Global::g_pColorBuffer->endUpdate();
}

void Editor::updatePositions() {
    if(_current_layer == NULL) return;

    OpenGL::Global::g_pPositionBuffer->beginUpdate(_buffer.vindex, 0);

    // top left
    Storage::Geometry::buildQuad(OpenGL::Global::g_pPositionBuffer,
            glm::vec2((int)_current_layer->X - 5.0f, (int)_current_layer->Y - 5.0f),
            glm::vec2(10.0f, 10.0f), 0.2f);

    // top right
    Storage::Geometry::buildQuad(OpenGL::Global::g_pPositionBuffer,
            glm::vec2(
                    (int)_current_layer->X + _current_layer->Width - 5.0f,
                    (int)_current_layer->Y - 5.0f),
            glm::vec2(10.0f, 10.0f), 0.2f);

    // bottom left
    Storage::Geometry::buildQuad(OpenGL::Global::g_pPositionBuffer,
            glm::vec2(
                    (int)_current_layer->X - 5.0f,
                    (int)_current_layer->Y + _current_layer->Height - 5.0f),
            glm::vec2(10.0f, 10.0f), 0.2f);

    // bottom right
    Storage::Geometry::buildQuad(OpenGL::Global::g_pPositionBuffer,
            glm::vec2(
                    (int)_current_layer->X + _current_layer->Width - 5.0f,
                    (int)_current_layer->Y + _current_layer->Height - 5.0f),
            glm::vec2(10.0f, 10.0f), 0.2f);

    OpenGL::Global::g_pPositionBuffer->endUpdate();
}

// ------------------------------------------------------------------