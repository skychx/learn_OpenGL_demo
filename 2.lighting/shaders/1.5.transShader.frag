#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    // 两张图片混合
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}