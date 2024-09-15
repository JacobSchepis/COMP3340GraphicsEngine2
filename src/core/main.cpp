#include <GL/glew.h>
#include <SDL/SDL.h>
#include <iostream>
#include "shaders/Shader.h"
#include "graphics/Renderer.h"

#include <vector>

#include "components/Mesh.h"
#include "components/Transform.h"
#include "components/Camera.h"

#include "systems/MonobehaviorManager.h"

#include "input/InputManager.h"

#include <glm/gtc/matrix_transform.hpp>

#include "scripts/CameraController.h"

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

    return true;
}

// Function to handle the render loop
void runRenderLoop(SDL_Window* window) {
    bool running = true;
    SDL_Event event;

    Shader* shader = new Shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    Renderer renderer = Renderer();

#pragma region Creating cubes
    std::vector<float> vertices = {
        // Positions         
        -0.5f, -0.5f, -0.5f, // 0: Bottom-left-back
         0.5f, -0.5f, -0.5f, // 1: Bottom-right-back
         0.5f,  0.5f, -0.5f, // 2: Top-right-back
        -0.5f,  0.5f, -0.5f, // 3: Top-left-back
        -0.5f, -0.5f,  0.5f, // 4: Bottom-left-front
         0.5f, -0.5f,  0.5f, // 5: Bottom-right-front
         0.5f,  0.5f,  0.5f, // 6: Top-right-front
        -0.5f,  0.5f,  0.5f  // 7: Top-left-front
    };


    std::vector<GLuint> indices = {
        // Back face
        0, 1, 2, 2, 3, 0,
        // Front face
        4, 5, 6, 6, 7, 4,
        // Left face
        0, 3, 7, 7, 4, 0,
        // Right face
        1, 5, 6, 6, 2, 1,
        // Bottom face
        0, 1, 5, 5, 4, 0,
        // Top face
        3, 2, 6, 6, 7, 3
    };


    Entity entity = Entity();
    entity.addComponent<Mesh>(vertices, indices);

    Entity entity1 = Entity();
    entity1.addComponent<Mesh>(vertices, indices);

    Mesh* meshPtr = entity.getComponent<Mesh>();
    Mesh* meshPtr1 = entity1.getComponent<Mesh>();

    entity1.getComponent<Transform>()->position = glm::vec3(1, 1, 0);
    entity1.getComponent<Transform>()->updateModelMatrix();

    renderer.queueMeshIntoBufferObject(meshPtr);
    renderer.queueMeshIntoBufferObject(meshPtr1);


    renderer.pushMeshesToBuffer();

#pragma endregion
  

#pragma region Creating Camera

    Entity camera = Entity();
    camera.addComponent<Camera>(60.0f, 4.0f / 3.0f, 0.1f, 20.0f);
    camera.addComponent<CameraController>();

    Camera* camPtr = camera.getComponent<Camera>();


    Transform* transformPtr = camera.getComponent<Transform>();


    transformPtr->position = glm::vec3(0, 0, 5);

    renderer.setActiveCamera(camPtr);

#pragma endregion

    MonobehaviorManager::Instance().awake();
    MonobehaviorManager::Instance().start();

    while (running) {
        
        InputManager::Instance().processInputs();
        if (InputManager::Instance().getApplicationQuit())
            SDL_Quit();

        MonobehaviorManager::Instance().update();

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
