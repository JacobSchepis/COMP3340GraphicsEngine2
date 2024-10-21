#include <GL/glew.h>
#include <SDL/SDL.h>
#include <iostream>
#include "shaders/Shader.h"
#include "graphics/Renderer.h"

#include <vector>

#include "components/Camera.h"
#include "components/rendering/Model.h"

#include "systems/MonobehaviorManager.h"

#include "input/InputManager.h"

#include <glm/gtc/matrix_transform.hpp>

#include "scripts/CameraController.h"

#include "components/rendering/Light.hpp"

#include "components/LightingManager.h"

#include <SDL/SDL_opengl.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Function to initialize SDL and create an OpenGL context
bool initSDL(SDL_Window** window, SDL_GLContext* context) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    // Set OpenGL version and profile (for core profile)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create an SDL window
    *window = SDL_CreateWindow("OpenGL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!*window) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create an OpenGL context
    *context = SDL_GL_CreateContext(*window);
    if (!*context) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE; // Enable modern OpenGL
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    // Enable V-sync
    SDL_GL_SetSwapInterval(1);

    // Set the viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    stbi_set_flip_vertically_on_load(true);


    return true;
}

void runRenderLoop(SDL_Window* window) {

#pragma region shader creation

    Shader* shader = new Shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    Renderer renderer = Renderer();

#pragma endregion

#pragma region creating entity

    Entity newEntity = Entity();

    char* filePath = "../../../resources/models/backpack/backpack.obj";
    newEntity.addComponent<Model>(filePath);

    Model* model = newEntity.getComponent<Model>();

    renderer.addModel(model);

#pragma endregion

#pragma region creating lightSource entity

    LightingManager lightManager;
    //Light(LightType type, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    Light staticLight("DIRECTIONAL", glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), true);
    lightManager.addLight(staticLight);
    // initialize the static light
    shader->Use();
    lightManager.initialiseStaticLighting(shader);

    Light dynamicLight("DIRECTIONAL", glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), false);
    lightManager.addLight(dynamicLight);


#pragma endregion

#pragma region Creating Camera

    Entity camera = Entity();
    camera.addComponent<Camera>(60.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
    camera.addComponent<CameraController>();

    Camera* camPtr = camera.getComponent<Camera>();


    Transform* transformPtr = camera.getComponent<Transform>();


    transformPtr->position = glm::vec3(0, 1, 0);

    renderer.setActiveCamera(camPtr);

#pragma endregion

    MonobehaviorManager::Instance().awake();
    MonobehaviorManager::Instance().start();

    bool running = true;
    SDL_Event event;

    while (running) {
        
        InputManager::Instance().processInputs();
        if (InputManager::Instance().getApplicationQuit())
            SDL_Quit();

        MonobehaviorManager::Instance().update();

        float currentTime = SDL_GetTicks() / 1000.0f;
        // update all dynamic light source
        lightManager.updateDynamicLighting(shader, currentTime);

        renderer.render(shader);

        SDL_GL_SwapWindow(window);
    }
}

// Function to clean up SDL and OpenGL context
void cleanup(SDL_Window* window, SDL_GLContext context) {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_GLContext context;

    if (!initSDL(&window, &context)) {
        std::cerr << "Failed to initialize" << std::endl;
        return -1;
    }

    runRenderLoop(window);
    cleanup(window, context);

    return 0;
}
