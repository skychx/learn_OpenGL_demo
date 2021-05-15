#version 330 core
out vec4 FragColor;

//in vec2 TexCoords;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

//uniform sampler2D texture1;

void main() {
    float ratio = 1.00 / 1.52;
    // 入射向量：摄像机位置 - 当前坐标
    vec3 I = normalize(Position - cameraPos);
    // 折射向量：GLSL 函数 refract 计算通过法线和入射向量计算折射向量
    vec3 R = refract(I, normalize(Normal), ratio);
    // 拿到反射向量对应的天空盒的颜色，作为当前片段的颜色
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}