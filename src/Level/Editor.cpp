#include <Level/Editor.h>
#include <Common/Emscripten.h>
#include <OpenGL/Global.h>
#include <Common/LiveLog/Builder.h>
#include <Common/LiveLog/CommonReflections.h>
#include <Storage/Geometry.h>
#include <Control/Mouse.h>
#include <OpenGL/Helper.h>
#include <Light/Renderer.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using namespace Level;

Editor* Editor::Instance = NULL;

#define UPDATE_TYPE_POS 0
#define UPDATE_TYPE_COLOR 1
#define UPDATE_TYPE_CAM 2
#define UPDATE_TYPE_REM_LAYER 3
#define UPDATE_TYPE_NEW_LAYER 4
#define UPDATE_TYPE_TEXTURE_CHANGED 5
#define UPDATE_TYPE_NEW_LIGHT 6
#define UPDATE_TYPE_LIGHT_POS 7
#define UPDATE_TYPE_LIGHT_COL 8
#define UPDATE_TYPE_LIGHT_STR 9

void editor_update_vals(int type) {
	switch(type) {
        case UPDATE_TYPE_POS: {
            Editor::Instance->getCurrentLayer()->X = EM_ASM_INT_V({return editor_ui_instance.layer.x; });
            Editor::Instance->getCurrentLayer()->Y = EM_ASM_INT_V({return editor_ui_instance.layer.y; });
            Editor::Instance->getCurrentLayer()->Z = EM_ASM_INT_V({return editor_ui_instance.layer.z; });
            Editor::Instance->getCurrentLayer()->Width = EM_ASM_INT_V({return editor_ui_instance.layer.width; });
            Editor::Instance->getCurrentLayer()->Height = EM_ASM_INT_V({return editor_ui_instance.layer.height; });
            Editor::Instance->getCurrentLayer()->Rotation = EM_ASM_INT_V({return editor_ui_instance.layer.rotation; });
            Editor::Instance->updatePositions();
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

            Common::LiveLog::Builder builder(LOG_EDITOR, LOG_TYPE_WARNING);
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
            layer->TextureCoord = glm::vec4(0.0f, 0.0f, 10.0f, 10.0f);
            layer->Base = Editor::Instance->getCurrentLevel();
            layer->Renderer = new Level::Renderer::Layer(layer);
            layer->updateChanges();
            Editor::Instance->getCurrentLevel()->Layers.push_back(layer);
            Editor::Instance->setCurrentLayer(layer);
            Editor::Instance->updateJsPositions();
            break;
        };
        case UPDATE_TYPE_TEXTURE_CHANGED: {
            glm::vec4 pos;
            pos.x = EM_ASM_INT_V({ return editor_ui_instance.layer.texture_pos.x; });
            pos.y = EM_ASM_INT_V({ return editor_ui_instance.layer.texture_pos.y; });
            pos.z = EM_ASM_INT_V({ return editor_ui_instance.layer.texture_pos.width; });
            pos.w = EM_ASM_INT_V({ return editor_ui_instance.layer.texture_pos.height; });
            Editor::Instance->getCurrentLayer()->TextureCoord = pos;
            break;
        };
        case UPDATE_TYPE_NEW_LIGHT: {
            auto light = new Light::Model();
            light->X = 100;
            light->Y = 100;
            light->Z = 1;
            light->Strength = 50;
            light->Radius = 100;
            light->Base = Editor::Instance->getCurrentLevel();
            light->Renderer = new Light::Renderer(light);
            light->updateChanges();
            Editor::Instance->getCurrentLevel()->Lights.push_back(light);
            Editor::Instance->setCurrentLight(light);
            Editor::Instance->updateJsPositions();
            break;
        };
        case UPDATE_TYPE_LIGHT_POS: {
            auto light = Editor::Instance->getCurrentLight();
            light->X = EM_ASM_INT_V({ return editor_ui_instance.light.x; });
            light->Y = EM_ASM_INT_V({ return editor_ui_instance.light.y; });
            light->Z = EM_ASM_INT_V({ return editor_ui_instance.light.z; });
            light->Radius = EM_ASM_INT_V({ return editor_ui_instance.light.radius; });
            light->updateChanges();
            Editor::Instance->updatePositions();
            break;
        };
        case UPDATE_TYPE_LIGHT_STR: {
            Editor::Instance->getCurrentLight()->Strength = EM_ASM_INT_V({ return editor_ui_instance.light.strength; });
            Editor::Instance->getCurrentLight()->updateChanges();
            break;
        };
        case UPDATE_TYPE_LIGHT_COL: {
            auto light = Editor::Instance->getCurrentLight();
            glm::vec4 new_col(1.0f);
            new_col.r = (float) EM_ASM_DOUBLE_V({ return editor_ui_instance.light.color[0]; });
            new_col.g = (float) EM_ASM_DOUBLE_V({ return editor_ui_instance.light.color[1]; });
            new_col.b = (float) EM_ASM_DOUBLE_V({ return editor_ui_instance.light.color[2]; });
            light->Color = new_col;
            light->updateChanges();
            break;
        };
    }
    Editor::Instance->getCurrentLayer()->updateChanges();
}

// ------------------------------------------------------------------

Editor::Editor()
    : _vertex_buffer(32, GL_ARRAY_BUFFER),
      _color_buffer(32, GL_ARRAY_BUFFER),
      _index_buffer(48, GL_ELEMENT_ARRAY_BUFFER)
{
    _color_program = OpenGL::Helper::createProgramFromFiles("colored");
    _render_system = new OpenGL::RenderSystem(_color_program, &_index_buffer);
    _render_system->addGpuBuffer("vPosition", &_vertex_buffer);
    _render_system->addGpuBuffer("vColor", &_color_buffer);
    _buffer = _render_system->requestCombined(8*4, 6*8);

    initial_buffer();

	_isActivated = false;
	Instance = this;
}

Editor::~Editor() {
    delete _render_system;
}

// ------------------------------------------------------------------
bool Editor::isEditing() {
    return _isActivated;
}

void Editor::setCurrentLevel(Model::Base *lvl) {
    _current_level = lvl;
    _current_layer = lvl->Layers[0];
}

void Editor::setCurrentLayer(Model::Layer *layer) {
    _current_layer = layer;
    _current_light = NULL;
    _current_type = 0;
    if(_current_layer != NULL) updatePositions();
    EM_ASM({editor_ui_instance.setLayerMode();});
}

void Editor::setCurrentLight(Light::Model* light) {
    _current_light = light;
    _current_layer = NULL;
    _current_type = 1;
    if(_current_light != NULL) updatePositions();
    EM_ASM({editor_ui_instance.setLightMode();});
}

// ------------------------------------------------------------------


void Editor::onClick() {
    if(!_isActivated) return;

    int mx = Control::Mouse::X + OpenGL::Global::g_pCamera->X;
    int my = Control::Mouse::Y + OpenGL::Global::g_pCamera->Y;

    struct collision {
        int mx;
        int my;

        bool collides(float px, float py, float width, float height, float rotation, float z) {
            // parallax correction
            float cam_offset = OpenGL::Global::g_pCamera->X + OpenGL::Global::g_Width / 2.0f;
            float origin = (float)px + width / 2.0f;
            float distance = cam_offset - origin;
            float x_offset = (distance/100.0f) * (z);
            float x = px + x_offset;
            float y = py;

            // rotate cursor
            glm::mat4 rot_matrix = glm::rotate(glm::radians(-(float)rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::vec2 rot_origin = glm::vec2(x + width / 2.0f, y + height / 2.0f);
            glm::vec2 pos = glm::vec2((float)mx, (float)my) - rot_origin;
            glm::vec4 trans = rot_matrix * glm::vec4(pos, 0.0f, 1.0f);
            trans = trans / trans.w;
            pos = trans.xy();
            pos += rot_origin;

            // check collision
            if(x <= pos.x && pos.x <= (x + width)) {
                if(y <= pos.y && pos.y <= (y + height)) {
                    return true;
                }
            }

            return false;
        }
    };

    collision col;
    col.mx = mx;
    col.my = my;

    // layers
    for(auto iter = _current_level->Layers.begin(); iter != _current_level->Layers.end(); iter++) {
        auto ptr = (*iter);
        if(col.collides(ptr->X, ptr->Y, ptr->Width, ptr->Height, ptr->Rotation, (float)ptr->Z - 1.0f)) {
            setCurrentLayer(ptr);
            updateJsPositions();
            return;
        }
    }
    _current_layer = NULL;

    // lights
    for(auto iter = _current_level->Lights.begin(); iter != _current_level->Lights.end(); iter++) {
        auto ptr = (*iter);
        if(col.collides(ptr->X - ptr->Radius, ptr->Y - ptr->Radius, ptr->Radius * 2.0f, ptr->Radius * 2.0f, 0.0f, (float)ptr->Z - 1.0f)) {
            setCurrentLight(ptr);
            updateJsPositions();
            return;
        }
    }
    _current_light = NULL;
}

void Editor::onDrag(int x, int y, int state, int mods) {
    if(!_isActivated) return;

    int difX = Control::Mouse::X - x;
    int difY = Control::Mouse::Y - y;

    if(mods & GLFW_MOD_CONTROL) {
        OpenGL::Global::g_pCamera->X -= difX;
        OpenGL::Global::g_pCamera->Y -= difY;

        if(OpenGL::Global::g_pCamera->X > INT_MAX) {
            OpenGL::Global::g_pCamera->X = 0;
        }
        if(OpenGL::Global::g_pCamera->Y > INT_MAX) {
            OpenGL::Global::g_pCamera->Y = 0;
        }
    } else {
        if(state == CONTROL_MOUSE_DRAG_BEGIN) onClick();
        if(_current_type == 0) {
            if (_current_layer == NULL) return;

            _current_layer->X += difX;
            _current_layer->Y += difY;
            _current_layer->updateChanges();
        }
        else if(_current_type == 1) {
            if (_current_light == NULL) return;

            _current_light->X += difX;
            _current_light->Y += difY;
            _current_light->updateChanges();
        }
        updatePositions();
    }

    if(state == CONTROL_MOUSE_DRAG_END) updateJsPositions();
}

// ------------------------------------------------------------------

void Editor::updateJsPositions() {
    // Camera
    EM_ASM_INT({
        editor_ui_instance.camera.x = $0;
        editor_ui_instance.camera.y = $1;
        editor_ui_instance.refreshCamera();
    }, (int)OpenGL::Global::g_pCamera->X, (int)OpenGL::Global::g_pCamera->Y);

    // Layer
	if (_current_type == 0 && _current_layer != NULL) {
        EM_ASM_INT({
            editor_ui_instance.layer.x = $0;
            editor_ui_instance.layer.y = $1;
            editor_ui_instance.layer.z = $2;
            editor_ui_instance.layer.width = $3;
            editor_ui_instance.layer.height = $4;
            editor_ui_instance.layer.rotation = $5;
            editor_ui_instance.refreshLayer();
        }, (int)_current_layer->X, (int)_current_layer->Y, (int)_current_layer->Z,
                (int)_current_layer->Width, (int)_current_layer->Height, (int)_current_layer->Rotation);
    }

    // Light
    if(_current_type == 1 && _current_light != NULL) {
        EM_ASM_INT({
            editor_ui_instance.light.x = $0;
            editor_ui_instance.light.y = $1;
            editor_ui_instance.light.z = $2;
            editor_ui_instance.light.radius = $3;
            editor_ui_instance.light.strength = $4;
            editor_ui_instance.light.color[0] = $5 / 255.0;
            editor_ui_instance.light.color[1] = $6 / 255.0;
            editor_ui_instance.light.color[2] = $7 / 255.0;
            editor_ui_instance.refreshLight();
        }, (int)_current_light->X, (int)_current_light->Y, (int)_current_light->Z,
                (int)_current_light->Radius, (int)_current_light->Strength,
                (int)(_current_light->Color->r * 255.0f), (int)(_current_light->Color->g * 255.0f), (int)(_current_light->Color->b * 255.0f));
    }
}

void Editor::toggle() {
    updateJsPositions();
	EM_ASM({
        editor_ui_instance.setMode(1);
        editor_ui_instance.toggle();
	});

	_isActivated = !_isActivated;

    if(_isActivated) updatePositions();
}

void Editor::render() {
    if(_isActivated && (_current_layer != NULL || _current_light != NULL)) {
        // QUICK-FIX
        updatePositions(); // NOTE: when camera changes we have to update anyway
        unsigned int loc = glGetUniformLocation(_color_program, "mModifier");
        glUseProgram(_color_program);
        glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)OpenGL::Global::getModifierMatrix());

        OpenGL::Global::getModifierMatrix();
        _index_buffer.uploadChanges();
        _vertex_buffer.uploadChanges();
        _color_buffer.uploadChanges();
        
        _render_system->enqueueRenderJob(_buffer.index, 8*6);
        _render_system->render();
    }
}

// ------------------------------------------------------------------

// black with some transparency
#define SELECTION_COLOR 0.86f, 0.21f, 1.0f
#define SELECTION_ALPHA 1.0f

void Editor::initial_buffer() {
    // Indices
    _index_buffer.beginUpdate(_buffer.index, 0);
    int offset = 0;
    for(int i = 0; i < 8; i++) {
        *_index_buffer.next() = 0 + _buffer.vindex + offset;
        *_index_buffer.next() = 1 + _buffer.vindex + offset;
        *_index_buffer.next() = 2 + _buffer.vindex + offset;
        *_index_buffer.next() = 2 + _buffer.vindex + offset;
        *_index_buffer.next() = 3 + _buffer.vindex + offset;
        *_index_buffer.next() = 0 + _buffer.vindex + offset;
        offset += 4;
    }
    _index_buffer.endUpdate();

    // Colors
    _color_buffer.beginUpdate(_buffer.vindex, 0);
    for(int i = 0; i < 8; i++) {
        glm::vec4* color = _color_buffer.next();
        *color = glm::vec4(SELECTION_COLOR, SELECTION_ALPHA);

        color = _color_buffer.next();
        *color = glm::vec4(SELECTION_COLOR, SELECTION_ALPHA);

        color = _color_buffer.next();
        *color = glm::vec4(SELECTION_COLOR, SELECTION_ALPHA);

        color = _color_buffer.next();
        *color = glm::vec4(SELECTION_COLOR, SELECTION_ALPHA);
    }
    _color_buffer.endUpdate();
}

void Editor::updatePositions() {
    if(_current_type == 0 && _current_layer == NULL) return;
    if(_current_type == 1 && _current_light == NULL) return;

    _vertex_buffer.beginUpdate(_buffer.vindex, 0);

    Storage::GeometryBuilder geom(&_vertex_buffer);

    struct local {
        static void buildEditorQuads(float px, float py, float width, float height, float rotation, float z, Storage::GeometryBuilder& geom) {
            // Calculate offset due to parallax scrolling
            float cam_offset = OpenGL::Global::g_pCamera->X + OpenGL::Global::g_Width / 2.0f;
            float origin = (float)px + width/ 2.0f;
            float distance = cam_offset - origin;
            geom.setOffset(glm::vec2((distance/100.0f) * ((float)z-1.0f), 0.0f));

            // Edge points
            geom.setSize(glm::vec2(10.0f, 10.0f));
            geom.setZOrder(0.2f);
            geom.setRotation(rotation);
            geom.setRotationOrigin(glm::vec2(
                    px + width / 2.0f,
                    py + height / 2.0f));

            // top left
            geom.setPosition(glm::vec2((int)px - 5.0f, (int)py - 5.0f));
            geom.buildQuad();

            // top right
            geom.setPosition(glm::vec2(
                    (int)px + width - 5.0f,
                    (int)py - 5.0f));
            geom.buildQuad();

            // bottom left
            geom.setPosition(glm::vec2(
                    (int)px - 5.0f,
                    (int)py + height - 5.0f));
            geom.buildQuad();

            // bottom right
            geom.setPosition(glm::vec2(
                    (int)px + width - 5.0f,
                    (int)py + height - 5.0f));
            geom.buildQuad();

            // Selection
            // Left
            geom.setPosition(glm::vec2((int)px - 2.5f, (int)py));
            geom.setSize(glm::vec2(5.0f, height + 2.5f));
            geom.buildQuad();

            // Right
            geom.setPosition(glm::vec2((int)px + width - 2.5f, (int)py));
            geom.buildQuad();

            // Top
            geom.setPosition(glm::vec2((int)px, (int)py - 2.5f));
            geom.setSize(glm::vec2(width + 2.5f, 5.0f));
            geom.buildQuad();

            // Bottom
            geom.setPosition(glm::vec2((int)px, py + height - 2.5f));
            geom.buildQuad();

        }
    };

    if(_current_type == 0) {
        local::buildEditorQuads(_current_layer->X,
                _current_layer->Y,
                _current_layer->Width,
                _current_layer->Height,
                _current_layer->Rotation,
                _current_layer->Z, geom);

    } else if( _current_type == 1) {
        local::buildEditorQuads(_current_light->X - _current_light->Radius,
                _current_light->Y - _current_light->Radius,
                _current_light->Radius * 2.0f,
                _current_light->Radius * 2.0f,
                0.0f,
                _current_light->Z, geom);
    }

    _vertex_buffer.endUpdate();
}

// ------------------------------------------------------------------