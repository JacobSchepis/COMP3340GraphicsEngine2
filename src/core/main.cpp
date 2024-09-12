#include <GL/glew.h>
#include <SDL/SDL.h>
#include <iostream>
#include "shaders/Shader.h"
#include "graphics/Renderer.h"

#include <vector>

#include "graphics/BufferObject.h"
#include "graphics/MeshAttributeFlags.h"
#include "graphics/Mesh.h"

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

#pragma region MyRegion
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



    MeshAttributeFlags flags = MeshAttributeFlags::None;

    Mesh mesh1 = Mesh(vertices, indices, glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Mesh mesh2 = Mesh(vertices, indices, glm::vec3(-1, 0, 0), glm::vec3(10, 0, 0), glm::vec3(1, 1, 1));


    renderer.queueMeshIntoBufferObject(&mesh1);
    renderer.queueMeshIntoBufferObject(&mesh2);

    
    renderer.pushMeshesToBuffer();

#pragma endregion
    
    while (running) {
        // Handle events
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
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
