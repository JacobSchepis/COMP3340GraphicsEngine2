#include <GL/glew.h>
#include <SDL/SDL.h>
#include <iostream>
#include "shaders/Shader.h"
#include "graphics/Renderer.h"

#include <vector>

#include "components/rendering/Mesh.hpp"
#include "components/Transform.h"
#include "components/Camera.h"

#include "systems/MonobehaviorManager.h"

#include "input/InputManager.h"

#include <glm/gtc/matrix_transform.hpp>

#include "scripts/CameraController.h"

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

    return true;
}

GLuint loadTextureFromFile(const std::string& path) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    std::cout << "something" << std::endl;

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    //std::cout << data << std::endl;
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    }
    else {
        std::string failureReason = stbi_failure_reason();
        std::cerr << "Failed to load texture: " << failureReason << std::endl;
    }

    stbi_image_free(data);
    return textureId;
}

// Function to handle the render loop
void runRenderLoop(SDL_Window* window) {
    bool running = true;
    SDL_Event event;

    Shader* shader = new Shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    Renderer renderer = Renderer();

#pragma region Creating cubes
    std::vector<Vertex> vertices = {
        // Position                 // Normal              // Texture Coordinates
        {{-0.5f, -0.5f,  0.5f},     {0.0f,  0.0f,  1.0f},  {0.0f, 0.0f}}, // Front-bottom-left
        {{ 0.5f, -0.5f,  0.5f},     {0.0f,  0.0f,  1.0f},  {1.0f, 0.0f}}, // Front-bottom-right
        {{ 0.5f,  0.5f,  0.5f},     {0.0f,  0.0f,  1.0f},  {1.0f, 1.0f}}, // Front-top-right
        {{-0.5f,  0.5f,  0.5f},     {0.0f,  0.0f,  1.0f},  {0.0f, 1.0f}}, // Front-top-left

        {{-0.5f, -0.5f, -0.5f},     {0.0f,  0.0f, -1.0f},  {1.0f, 2.0f}}, // Back-bottom-left
        {{ 0.5f, -0.5f, -0.5f},     {0.0f,  0.0f, -1.0f},  {2.0f, 1.0f}}, // Back-bottom-right
        {{ 0.5f,  0.5f, -0.5f},     {0.0f,  0.0f, -1.0f},  {2.0f, 2.0f}}, // Back-top-right
        {{-0.5f,  0.5f, -0.5f},     {0.0f,  0.0f, -1.0f},  {1.0f, 2.0f}}  // Back-top-left
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

    auto* mesh = entity.getComponent<Mesh>();
    mesh->setupMesh();

    entity.addComponent<Material>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
    entity.addComponent<Texture>(loadTextureFromFile(
        "resources/images/GrassTexture.jpg"),
        "resources/images/GrassTexture.jpg");
    entity.addComponent<MeshRenderer>();
    auto* meshRenderer = entity.getComponent<MeshRenderer>();
    meshRenderer->setup();

    renderer.addMeshRenderer(meshRenderer);


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

    glEnable(GL_DEPTH_TEST);

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
