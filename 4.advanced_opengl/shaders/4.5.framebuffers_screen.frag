#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

void main() {
    // 正常绘制
    // FragColor = texture(screenTexture, TexCoords);
    // 反相
    // FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);

    // 平均灰度
    // FragColor = texture(screenTexture, TexCoords);
    // float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    // FragColor = vec4(average, average, average, 1.0);

    // 加权灰度（绿色权重加大，蓝色权重降低，更符合人眼的认知）
    // FragColor = texture(screenTexture, TexCoords);
    // float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    // FragColor = vec4(average, average, average, 1.0);

    // 偏移矩阵
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

    // 锐化矩阵
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    // 将每个偏移量加到当前纹理坐标上，获取需要采样的纹理
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }

    // 将这些纹理值乘以加权的核值
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++) {
        col += sampleTex[i] * kernel[i];
    }

    FragColor = vec4(col, 1.0);
}