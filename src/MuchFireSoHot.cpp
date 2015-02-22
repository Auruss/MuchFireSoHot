#include <cstdio>
#include <cstring>
#include <string>
#include <cstdlib>
#include <stdlib.h>

#include <emscripten/emscripten.h>

#include <Common/GameTime.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Level/Model/Base.h>

#include <Control/Mapper.h>
#include <Control/Global.h>
#include <Control/Mouse.h>

#include <OpenGL/Global.h>

#include <Common/LiveLog/Reflection.h>
#include <Common/LiveLog/Builder.h>
#include <Common/LiveLog/CommonReflections.h>
#include <OpenGL/Helper.h>

GameTimeObj fps_debugger;


extern "C" void toggle_fullscreen();
extern "C" void set_resolution(int x, int y);

// events required for frontend
extern "C" void game_build_status(int status);  // 0: ready 1: errors
extern "C" void game_session_status(int status); // 0: ready 1: errors
extern "C" void game_session_percentage_changed(int percentage);


Level::Model::Base* current_level = NULL;

Control::Mapper* mapper = NULL;
Control::Global* global_controls = NULL;


struct FpsLog {
    int fps;
};
static Common::LiveLog::ReflObject FpsLogRefl;


// prepare test system
void prepare_test() {
    // load level texture

    // create test level
    current_level = new Level::Model::Base();
    current_level->ChunksX = 5;
    current_level->ChunksY = 5;
    current_level->ChunkScale = 10;
    current_level->LevelName = "Developer Test Level";
    current_level->Renderer = new Level::Renderer::Base(current_level);
    current_level->Renderer->TextureId =
            OpenGL::Helper::createTextureFromFile("data/compiled/default-map.png", current_level->Renderer->TextureSize);

    // test layer
    auto test_layer = new Level::Model::Layer();
    test_layer->X = 10;
    test_layer->Y = 10;
    test_layer->Z = 1;
    test_layer->Width = 10;
    test_layer->Height = 10;
    test_layer->TextureCoord = glm::vec4(0.0f, 0.0f, 10.0f, 10.0f);
    test_layer->Renderer = new Level::Renderer::Layer(test_layer);
    test_layer->Base = current_level;
    test_layer->updateChanges();

    auto test_layer2 = new Level::Model::Layer();
    test_layer2->X = 20;
    test_layer2->Y = 50;
    test_layer2->Z = 2;
    test_layer2->Width = 50;
    test_layer2->Height = 50;
    test_layer2->TextureCoord = glm::vec4(0.0f, 0.0f, 10.0f, 10.0f);
    test_layer2->Renderer = new Level::Renderer::Layer(test_layer2);
    test_layer2->Base = current_level;
    test_layer2->updateChanges();

    auto test_layer_full = new Level::Model::Layer();
    test_layer_full->X = 0;
    test_layer_full->Y = 0;
    test_layer_full->Z = 1;
    test_layer_full->Width = 500;
    test_layer_full->Height = 500;
    test_layer_full->TextureCoord = glm::vec4(0.0f, 0.0f, 10.0f, 10.0f);
    test_layer_full->Renderer = new Level::Renderer::Layer(test_layer_full);
    test_layer_full->Base = current_level;
    test_layer_full->updateChanges();

    // add layers
    current_level->Layers.push_back(test_layer);
    current_level->Layers.push_back(test_layer2);
    current_level->Layers.push_back(test_layer_full);

    // editor
    global_controls->LevelEditor->setCurrentLevel(current_level);
}


GLFWwindow* window;

//called each frame
void mainloop()
{
    // poll changes
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // update
    Common::GameTime::on_frame();
    /*if (Common::GameTime::tickEvery(10000, fps_debugger, false)) { // print every 10 seconds the fps
        printf("FPS Debugger Tick: %d\n", Common::GameTime::FPS);
    }*/
    if(Common::GameTime::tickEvery(500, fps_debugger, false)) {
        FpsLog log;
        log.fps = Common::GameTime::FPS;

        Common::LiveLog::Builder builder(LOG_STATS_FPS, LOG_TYPE_INFO);
        builder.setMessage("5 secs elapsed, time for logging");
        builder.addRefObj("fps", &FpsLogRefl, (void*)&log);
        builder.push();
    }

    current_level->Renderer->update();
    OpenGL::Global::update();

    // render
    current_level->Renderer->render();
    global_controls->LevelEditor->render();
    OpenGL::Global::render();

    glfwSwapBuffers(window);
}

void on_single_click() {
    global_controls->onSingleClick();
}

void on_drag(int x, int y, int state, int mods) {
    global_controls->onDrag(x, y, state, mods);
}

void on_glfw_key_input(GLFWwindow* window, int key, int scan_code, int action, int mods) {
    int mapped_action = mapper->mapGLFWinput(key, action, mods);

    global_controls->onAction(mapped_action);
}

void init_gl(int width, int height) {
    if (glfwInit() != GL_TRUE) {
        fprintf(stderr, "GLFW failed\n");
        exit(1);
    }

    window = glfwCreateWindow(width, height, "JS", NULL, NULL);
    if (window == NULL) {
        printf("glfwCreateWindow() failed\n");
        exit(1);
    }
    //glfwSwapInterval(1);
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if(err != GLEW_OK) {
        fprintf(stderr, "GLEW failed: %s\n", glewGetErrorString(err));
        exit(1);
    }

    printf("OpenGL Stats:\n");

    // Printing OpenGL information
    printf("\tVersion:  %s\n", glGetString(GL_VERSION));
    printf("\tGLSL:     %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("\tVendor:   %s\n", glGetString(GL_VENDOR));
    printf("\tRenderer: %s\n", glGetString(GL_RENDERER));

    // Print extensions
    GLint n = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);
    printf("\tExt.:     %d\n", n);


#ifdef WIN32
    for (GLint i = 0; i < n; i++)
    {
        printf("\tExt %d: %s\n", i, glGetStringi(GL_EXTENSIONS, i));
    }
#else
    printf("Extensions: %s\n", glGetString(GL_EXTENSIONS));
#endif


    glClearColor(0.4f, 0.1f, 0.3f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    Common::GameTime::initialize();
    OpenGL::Global::init(width, height);

    // GLFW input
    glfwSetKeyCallback(window, on_glfw_key_input);
    glfwSetMouseButtonCallback(window, Control::Mouse::glfw_mouse_button);
    glfwSetCursorPosCallback(window, Control::Mouse::glfw_mouse_move);
    Control::Mouse::setOnSingleClick(on_single_click);
    Control::Mouse::setOnDrag(on_drag);
}

int main(int argc, char* argv [])
{
    Common::LiveLog::init_common_reflections();

    bool test_flag = false;
    bool fullscreen_flag = false;
    int resolution_x = 800;
    int resolution_y = 600;

    for(int i = 0; i < argc; i++) {
        if(std::strcmp(argv[i], "--developer-test") == 0) {
            test_flag = true;
        }
        if (std::strcmp(argv[i], "--fullscreen") == 0) {
            fullscreen_flag = true;
        }
        else if(std::strcmp(argv[i], "-rx") == 0) {
            resolution_x = std::stoi(argv[++i]);
        }
        else if (std::strcmp(argv[i], "-ry") == 0) {
            resolution_y = std::stoi(argv[++i]);
        }
    }

    init_gl(resolution_x, resolution_y);
    mapper = new Control::Mapper();
    global_controls = new Control::Global(test_flag);

    FpsLogRefl.init<FpsLog>();
    FpsLogRefl.addMember<int>("fps", offsetof(FpsLog, fps));
    if(FpsLogRefl.sizeOf() != sizeof(FpsLog)) {
        fprintf(stderr, "Reflection error!\n");
        return 1;
    }

    if (test_flag) {
        printf("Started developer test\n");
        prepare_test();
    }

    emscripten_set_main_loop(mainloop, 0, true);
    return 0;
}
