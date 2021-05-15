#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

// 几何着色器中声明相同的接口块
in VS_OUT {
    vec3 color;
} gs_in[];

// 为下个片段着色器阶段声明一个输出颜色向量
out vec3 fColor;

void build_house(vec4 position) {
    fColor = gs_in[0].color; // gs_in[0] 因为只有一个输入顶点
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // 1:bottom-left
    EmitVertex();
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); // 3:top-left
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); // 4:top-right
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0); // 5:top
    // 将最后一个顶点的颜色设置为白色，给屋顶落上一些雪
    fColor = vec3(1.0, 1.0, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main() {
    build_house(gl_in[0].gl_Position);
}