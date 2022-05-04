#version 460 core

const int NUM_LIGHTS = 1;

struct DirLight {
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct PointLight {
    vec4 position;
    vec4 attenuation;   // 0: constant, 1: linear, 2: quadratic
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct SpotLight {
    vec4 position;
    vec4 direction;
    vec4 cutoff;        // 0: cutoff, 1: outerCutoff
    vec4 attenuation;   // 0: constant, 1: linear, 2: quadratic
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

uniform int s_color;
uniform float value_color;
uniform sampler2D color;
uniform int s_normal;
uniform sampler2D normal;
uniform int s_diffuse;
uniform float value_diffuse;
uniform sampler2D diffuse;
uniform int s_specular;
uniform float value_specular;
uniform sampler2D specular;
uniform float shininess;

layout(std140) uniform CameraData{
    mat4 model;
    mat4 view;
    mat4 projection;
    vec4 viewPos;
};

layout(std140) uniform PointLights {
    PointLight pLights[NUM_POINT_LIGHTS]; // error can be ignored since the value is inserted at runtime
};

layout(std140) uniform DirectionalLights {
    DirLight dLights[NUM_DIRECTIONAL_LIGHTS]; // error can be ignored since the value is inserted at runtime
};

layout(std140) uniform SpotLights {
    SpotLight sLights[NUM_SPOT_LIGHTS]; // error can be ignored since the value is inserted at runtime
};

in vec4 fragColor;
in vec3 fragPos;
in vec3 fragNormal;
in vec2 texCoords;

out vec4 outColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    // properties
    vec3 norm = normalize(s_normal == 0 ? fragNormal : vec3(texture(normal, texCoords)));
    vec3 viewDir = normalize(viewPos.xyz - fragPos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    vec3 result = vec3(0, 0, 0);
    // phase 1: directional lights
    for (int i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++) // error can be ignored since the value is inserted at runtime
        result += CalcDirLight(dLights[i], norm, viewDir);
    // phase 2: point lights
    for (int i = 0; i < NUM_POINT_LIGHTS; i++) // error can be ignored since the value is inserted at runtime
        result += CalcPointLight(pLights[i], norm, fragPos, viewDir);
    // phase 3: spot light
    for (int i = 0; i < NUM_SPOT_LIGHTS; i++) // error can be ignored since the value is inserted at runtime
        result += CalcSpotLight(sLights[i], norm, fragPos, viewDir);
    
    outColor = vec4(vec3(texture(color, texCoords)) * result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction.xyz);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient = light.ambient.xyz * (s_diffuse == 0 ? vec3(value_diffuse) : vec3(texture(diffuse, texCoords)));
    vec3 diffuse = light.diffuse.xyz * diff * (s_diffuse == 0 ? vec3(value_diffuse) : vec3(texture(diffuse, texCoords)));
    vec3 specular = light.specular.xyz * spec * (s_specular == 0 ? vec3(value_specular) :vec3(texture(specular, texCoords)));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance +
        light.attenuation.z * (distance * distance));
    // combine results
    vec3 ambient = light.ambient.xyz * (s_diffuse == 0 ? vec3(value_diffuse) : vec3(texture(diffuse, texCoords)));
    vec3 diffuse = light.diffuse.xyz * diff * (s_diffuse == 0 ? vec3(value_diffuse) : vec3(texture(diffuse, texCoords)));
    vec3 specular = light.specular.xyz * spec * (s_specular == 0 ? vec3(value_specular) :vec3(texture(specular, texCoords)));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction.xyz));
    float epsilon = light.cutoff.x - light.cutoff.y;
    float intensity = clamp((theta - light.cutoff.y) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient.xyz * (s_diffuse == 0 ? vec3(value_diffuse) : vec3(texture(diffuse, texCoords)));
    vec3 diffuse = light.diffuse.xyz * diff * (s_diffuse == 0 ? vec3(value_diffuse) : vec3(texture(diffuse, texCoords)));
    vec3 specular = light.specular.xyz * spec * (s_specular == 0 ? vec3(value_specular) :vec3(texture(specular, texCoords)));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}