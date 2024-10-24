#version 330 core
layout (location = 0) in vec2 aPos;       // 2D 顶点位置输入
layout (location = 1) in vec2 aTexCoords; // 纹理坐标输入

out vec2 TexCoords;

uniform float cloudHeight;  // 云层的固定高度
uniform mat4 view;
uniform mat4 projection;

void main() {
    // 使用输入的2D坐标和固定高度创建3D世界空间位置
    vec3 worldPos = vec3(aPos.x, cloudHeight, aPos.y);
    
    // 应用模型、视图和投影变换
    gl_Position = projection * view * vec4(worldPos, 1.0);
    
    TexCoords = aTexCoords;
}
