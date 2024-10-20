cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(COMP3340GraphicsEngine2 VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for your header files (GLEW, SDL, and your own headers)
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/include/glew")
include_directories("${PROJECT_SOURCE_DIR}/include/SDL2")

# Add all source files from the src directory (automatically find .cpp files)
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/**/*.cpp")

# Add the executable and link it to the source files
add_executable(COMP3340GraphicsEngine2 ${SOURCES} 
"include/graphics/Vertex.hpp"

"include/graphics/Renderer.h"                   "src/graphics/Renderer.cpp" 
  

"include/components/abstract/Monobehavior.hpp"  
"include/components/abstract/IComponent.hpp" 

"include/graphics/Mesh.hpp" 
"include/graphics/Material.hpp"  
"include/graphics/Texture.hpp" 
"include/graphics/MeshRenderer.h"               "src/graphics/MeshRenderer.cpp"

"include/components/Camera.h"                   "src/components/Camera.cpp"
"include/components/Transform.h"                "src/components/Transform.cpp" 

"include/entities/Entity.h"                     "src/entities/Entity.cpp" 

"include/systems/MonobehaviorManager.h"         "src/systems/MonobehaviorManager.cpp" 

"include/scripts/CameraController.h"            "src/scripts/CameraController.cpp" 

"include/input/InputManager.h"                  "src/input/InputManager.cpp"

"include/components/rendering/Light.hpp"        "src/components/rendering/Light.cpp"

"include/util/Debug.h"                          "src/util/Debug.cpp"  

 "include/components/rendering/Model.h"         "src/components/rendering/Model.cpp" "include/util/Time.hpp"  "include/systems/LightingManager.h" "src/systems/LightingManager.cpp")
# Link OpenGL (use opengl32.lib directly for Windows)
if(WIN32)
    target_link_libraries(COMP3340GraphicsEngine2 opengl32.lib)
endif()

# Link the GLEW and SDL libraries from the lib folder
target_link_libraries(COMP3340GraphicsEngine2 "${PROJECT_SOURCE_DIR}/lib/glew32.lib")
target_link_libraries(COMP3340GraphicsEngine2 "${PROJECT_SOURCE_DIR}/lib/SDL2.lib")
target_link_libraries(COMP3340GraphicsEngine2 "${PROJECT_SOURCE_DIR}/lib/SDL2main.lib")
target_link_libraries(COMP3340GraphicsEngine2 "${PROJECT_SOURCE_DIR}/lib/assimp-vc143-mtd.lib")

# Copy necessary DLL files to the build directory for runtime (if on Windows)
if(WIN32)
    file(COPY "${PROJECT_SOURCE_DIR}/dll/glew32.dll" DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY "${PROJECT_SOURCE_DIR}/dll/SDL2.dll" DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY "${PROJECT_SOURCE_DIR}/dll/assimp-vc143-mtd.dll" DESTINATION ${CMAKE_BINARY_DIR})
endif()

# Copy shaders to the build directory (optional if you have shaders)
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
