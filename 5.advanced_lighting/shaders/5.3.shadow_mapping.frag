#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

// 通过对比同一个坐标在深度贴图和摄像机视角里的 z 值
// 计算是不是被遮挡
// 未被遮挡，返回 0，被遮挡，返回 1
float ShadowCalculation(vec4 fragPosLightSpace) {
    // perform perspective divide 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range 范围转换 [-1, 1] => [0, 1]
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    // 取得当前片段在光源视角下的深度
    float currentDepth = projCoords.z;

    // 阴影偏移，解决条纹阴影的问题
    // 像地板这样的表面几乎与光源垂直，得到的偏移就很小，而比如立方体的侧面这种表面得到的偏移就更大。
    // 参考：
    // 关于Shadow Mapping产生的Shadow Acne，我的理解是不是有问题？https://www.zhihu.com/question/49090321/answer/114217482
    // http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // 一个简单的 PCF（percentage-closer filtering）实现：
    // 从纹理像素四周对深度贴图采样，然后把结果平均起来
    float shadow = 0.0;

    // return currentDepth > closestDepth  ? 1.0 : 0.0;
    // textureSize返回一个给定采样器纹理的0级mipmap的vec2类型的宽和高
    // 用1除以它返回一个单独纹理像素的大小
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    // 对纹理坐标进行偏移，确保每个新样本，来自不同的深度值
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;

            // check whether current frag pos is in shadow
            // 检查当前片段是否在阴影中
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    // 这里我们获得 3x3 9 个值，所以求它们最后的平均数
    shadow /= 9.0;

    // 当一个点比光的远平面还要远时，它的投影坐标的z坐标大于1.0。
    // 这种情况下，GL_CLAMP_TO_BORDER环绕方式不起作用，
    // 因为我们把坐标的z元素和深度贴图的值进行了对比；它总是为大于1.0的z返回true。
    // 解决这个问题也很简单，只要投影向量的z坐标大于1.0，我们就把shadow的值强制设为0.0：
    if(projCoords.z > 1.0) {
        shadow = 0.0;
    }

    return shadow;
}

void main() {
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
    // Blinn-Phong Model
    // ===================
    // ambient
    vec3 ambient = 0.3 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    // calculate shadow
    // 当 fragment 在阴影中时是1.0，在阴影外是0.0
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    FragColor = vec4(lighting, 1.0);
}