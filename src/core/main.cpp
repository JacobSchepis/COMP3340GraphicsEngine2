#include <GL/glew.h>
#include <SDL/SDL.h>
#include <iostream>
#include "shaders/Shader.h"

#include <vector>
#include <graphics/TestCube.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

GLuint VBO, EBO;


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

void createObject(std::vector<TestCube>& testCubes) {
    std::vector<float> vertices;
    std::vector<GLuint> indices;

    // Combine vertices and adjust indices for each cube
    int offset = 0;
    for (auto& testCube : testCubes) {
        // Add the vertices of the current TestCube to the combined list
        vertices.insert(vertices.end(), testCube.vertices.begin(), testCube.vertices.end());

        // Add the indices, adjusting the offset for each object
        for (auto index : testCube.indices) {
            indices.push_back(index + offset);  // Offset the index based on previously added vertices
        }

        offset += testCube.vertices.size() / 3;  // Increase offset by the number of vertices
    }

    // Generate the VBO and EBO for the combined vertex and index data
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Now each TestCube will configure its own VAO
    for (auto& testCube : testCubes) {
        testCube.setupVAO(VBO, EBO);
    }
}


// Function to handle the render loop
void runRenderLoop(SDL_Window* window) {
    bool running = true;
    SDL_Event event;


    Shader* shader = new Shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

    std::vector<GLuint> indices = {
        0, 1, 2
    };

    std::vector<float> vertices1 = {
        -0.9f,  0.5f, 0.0f,   // Top left vertex
        -0.5f, -0.5f, 0.0f,   // Bottom right vertex
        -1.0f, -0.5f, 0.0f    // Bottom left vertex
    };

    TestCube testCube1 = TestCube(vertices1, indices);
    
    std::vector<float> vertices2 = {
        // Positions         // Texture coordinates (optional)
         0.5f,  0.5f, 0.0f,   // Top right vertex
         0.9f, -0.5f, 0.0f,   // Bottom right vertex
         0.0f, -0.5f, 0.0f    // Bottom left vertex
    };

    TestCube testCube2 = TestCube(vertices2, indices);

    std::vector<TestCube> testCubes = { testCube1, testCube2 };

    createObject(testCubes);

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Clear the screen with a color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->Use();
        for (auto& testCube : testCubes) {
            glBindVertexArray(testCube.VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        
        
        // Swap buffers
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
