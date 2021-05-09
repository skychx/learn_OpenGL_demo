#version 330 core

struct Material {
    vec3 ambient; // 环境光照下这个物体反射得是什么颜色
    vec3 diffuse; // 漫反射光照下物体的颜色
    vec3 specular; // 镜面光照对物体的颜色影响
    float shininess; // 镜面高光的散射/半径
};

// 一个光源对它的 ambient、diffuse 和 specular 光照有着不同的强度
struct Light {
    vec3 position;

    vec3 ambient; // 环境光照通常会设置为一个比较低的强度
    vec3 diffuse; // 光源的漫反射分量通常设置为光所具有的颜色，通常是一个比较明亮的白色
    vec3 specular; // 镜面光分量通常会保持为vec3(1.0)，以最大强度发光
};

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
    // 一、环境光照
    vec3 ambient = light.ambient * material.ambient;

    // 二、漫反射光照
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // 三、镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}