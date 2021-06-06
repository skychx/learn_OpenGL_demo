#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float heightScale;

// 普通视差贴图
//vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
//    float height =  texture(depthMap, texCoords).r; // 获取当前 fragment 的高度
//    // x 和 y 元素在切线空间中，viewDir 向量除以它的 z 元素，用 fragment 的高度对它进行缩放
//    vec2 p = viewDir.xy / viewDir.z * (height * heightScale);
//    return texCoords - p;
//}

// 陡峭视差映射：通过采样的数量提高精确度
//vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
//    // number of depth layers
//    // 定义层的数量：垂直看表面时使用更少的样本，以一定角度看时增加样本数量
//    const float minLayers = 8;
//    const float maxLayers = 32;
//    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
//
//    // calculate the size of each layer
//    // 计算每一层的深度
//    float layerDepth = 1.0 / numLayers;
//
//    // depth of current layer
//    float currentLayerDepth = 0.0;
//    // the amount to shift the texture coordinates per layer (from vector P)
//    // 计算纹理坐标偏移
//    vec2 P = viewDir.xy / viewDir.z * heightScale;
//    vec2 deltaTexCoords = P / numLayers;
//
//    // get initial values
//    vec2  currentTexCoords     = texCoords;
//    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
//
//    // 遍历所有层，从上开始，直到找到小于这一层的深度值的深度贴图值
//    while(currentLayerDepth < currentDepthMapValue) {
//        // shift texture coordinates along direction of P
//        currentTexCoords -= deltaTexCoords;
//        // get depthmap value at current texture coordinates
//        currentDepthMapValue = texture(depthMap, currentTexCoords).r;
//        // get depth of next layer
//        currentLayerDepth += layerDepth;
//    }
//
//    return currentTexCoords;
//}

// 视差遮蔽映射: 在高度触碰前和触碰后的深度值之间插值
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;

    while(currentLayerDepth < currentDepthMapValue) {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;
        // get depth of next layer
        currentLayerDepth += layerDepth;
    }

    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;

    // interpolation of texture coordinates
    // 做线性插值
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main() {
    // offset texture coordinates with Parallax Mapping
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = fs_in.TexCoords;

    texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);

    // 平面的边缘上，纹理坐标超出了 0 到 1 的范围进行采样，根据纹理的环绕方式导致了不真实的结果。
    // 解决的方法是当它超出默认纹理坐标范围进行采样的时候就丢弃这个 fragment
    if (texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0) {
        discard;
    }

    // obtain normal from normal map
    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    // get diffuse color
    vec3 color = texture(diffuseMap, texCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}