#version 460 core

const int NUM_LIGHTS = 1;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout(std140) uniform Material {
    vec4 color;
    float diffuse;
    float specular;
    float shininess;
} material;

layout(std140) uniform CameraData {
    mat4 model;
    mat4 view;
    mat4 projection;
};

const vec3 viewPos = vec3(0, 1, -6);


layout(std140) uniform Lights {
    PointLight pLights[NUM_LIGHTS];
    DirLight dLights[NUM_LIGHTS];
    SpotLight sLights[NUM_LIGHTS];
};

in vec4 fragColor;
in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

out vec4 outColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    // properties
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    vec3 result = vec3(0, 0, 0);
    /*
    // phase 1: directional lighting
    for (int i = 0; i < NUM_LIGHTS; i++)
        result += CalcDirLight(dLights[i], norm, viewDir);
    // phase 2: point lights
    for (int i = 0; i < NUM_LIGHTS; i++)
        result += CalcPointLight(pLights[i], norm, fragPos, viewDir);
    // phase 3: spot light
    for (int i = 0; i < NUM_LIGHTS; i++)
        result += CalcSpotLight(sLights[i], norm, fragPos, viewDir);
      */
    outColor = vec4(pLights[0].position, 1.0);
    //outColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(material.diffuse);
    vec3 diffuse = light.diffuse * diff * vec3(material.diffuse);
    vec3 specular = light.specular * spec * vec3(material.specular);
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(material.diffuse);
    vec3 diffuse = light.diffuse * diff * vec3(material.diffuse);
    vec3 specular = light.specular * spec * vec3(material.diffuse);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(material.diffuse);
    vec3 diffuse = light.diffuse * diff * vec3(material.diffuse);
    vec3 specular = light.specular * spec * vec3(material.specular);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}