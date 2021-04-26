#version 330 core
// 每个着色器都起始于一个版本声明，上面表示 OpenGL3。3 核心模式
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 transform;

void main() {
    gl_Position = vec4(aPos, 1.0);
    gl_Position = transform * vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}