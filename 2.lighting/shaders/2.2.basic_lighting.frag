#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
    // 一、环境光照
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // 二、漫反射光照
    vec3 norm = normalize(Normal); // 标准化法线向量
    vec3 lightDir = normalize(lightPos - FragPos); // 标准化入射光线
    float diff = max(dot(norm, lightDir), 0.0); // 计算法线和光线之间的夹角
    vec3 diffuse = diff * lightColor; // 算出漫反射光照

    // 三、镜面反射
    float specularStrength = 0.5; // 镜面强度变量
    vec3 viewDir = normalize(viewPos - FragPos); // 视线方向向量
    // reflect 函数要求第一个向量是从光源指向片段位置的向量，
    // 但是 lightDir 当前正好相反，是从片段指向光源（由先前我们计算lightDir向量时，减法的顺序决定）。
    // 为了保证我们得到正确的 reflect 向量，我们通过对 lightDir 向量取反来获得相反的方向。
    // 第二个参数要求是一个法向量，所以我们提供的是已标准化的 norm 向量
    vec3 reflectDir = reflect(-lightDir, norm); // 沿着法线轴的反射向量
    // 先计算视线方向与反射方向的点乘
    // 取它的32次幂（32是高光的反光度(Shininess)）
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}