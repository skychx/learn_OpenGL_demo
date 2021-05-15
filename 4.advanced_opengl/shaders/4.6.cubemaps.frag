#version 330 core
out vec4 FragColor;

//in vec2 TexCoords;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

//uniform sampler2D texture1;

void main() {
    // 入射向量：摄像机位置 - 当前坐标
    vec3 I = normalize(Position - cameraPos);
    // 反射向量：GLSL 函数 reflect 计算通过法线和入射向量计算发射向量
    vec3 R = reflect(I, normalize(Normal));
    // 拿到反射向量对应的天空盒的颜色，作为当前片段的颜色
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}