#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main() {
    vec4 texColor = texture(texture1, TexCoords);
    // GLSL 给了我们 discard 命令，一旦被调用，它就会保证片段不会被进一步处理
    // 在片段着色器中检测一个片段的 alpha 值是否低于某个阈值
    if (texColor.a < 0.1) {
        discard;
    }

    FragColor = texColor;
}