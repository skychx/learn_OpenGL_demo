#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

// 使用一个接口块将颜色属性发送到几何着色器中
out VS_OUT {
    vec3 color;
} vs_out;

void main() {
    vs_out.color = aColor;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}