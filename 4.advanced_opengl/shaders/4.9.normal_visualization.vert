#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 view;
uniform mat4 model;

// https://github.com/JoeyDeVries/LearnOpenGL/commit/3615cfce79fe68c9c6c26b91e5e4d6d87e5dd0a1#diff-0e243a524e4b1a7dc8d8b2a21cda446d06d0bafac5766a991017941b3476f535
void main() {
    // 法线矩阵
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = vec3(vec4(normalMatrix * aNormal, 0.0));
    gl_Position = view * model * vec4(aPos, 1.0);
}