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

layout(std140) uniform Material {
    vec4 color;
    vec4 data;          //0: diffuse, 1: specular, 2: shininess
} material;

layout(std140) uniform CameraData {
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

uniform samplerCube cubemap;
uniform int s_cubemap = 1;

in vec4 fragColor;
in vec3 fragPos;
in vec3 fragNormal;
in vec2 texCoords;

out vec4 outColor;

vec3 CubemapReflection(vec3 normal, vec3 directionToCam);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    // properties
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos.xyz - fragPos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    vec3 result = vec3(0, 0, 0);
    // phase 1: directional lighting

    for (int i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++) // error can be ignored since the value is inserted at runtime
        result += CalcDirLight(dLights[i], norm, viewDir);
    // phase 2: point lights
    for (int i = 0; i < NUM_POINT_LIGHTS; i++) // error can be ignored since the value is inserted at runtime
        result += CalcPointLight(pLights[i], norm, fragPos, viewDir);
    // phase 3: spot light
    for (int i = 0; i < NUM_SPOT_LIGHTS; i++) // error can be ignored since the value is inserted at runtime
        result += CalcSpotLight(sLights[i], norm, fragPos, viewDir);
    
    result += s_cubemap * CubemapReflection(norm, viewDir);

    outColor = vec4(result * material.color.xyz * vec3(fragColor), 1.0);
}


vec3 CubemapReflection(vec3 normal, vec3 directionToCam) {
	vec3 reflectDir = reflect(directionToCam, normal);
	return vec3(texture(cubemap, -reflectDir));
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    if(length(light.ambient) + length(light.diffuse) + length(light.specular) <= 0)
        return vec3(0);

    vec3 lightDir = normalize(-light.direction.xyz);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.data.z);
    // combine results
    vec3 ambient = light.ambient.xyz * vec3(material.data.x);
    vec3 diffuse = light.diffuse.xyz * diff * vec3(material.data.x);
    vec3 specular = light.specular.xyz * spec * vec3(material.data.y);
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    if(length(light.ambient) + length(light.diffuse) + length(light.specular) <= 0)
        return vec3(0);

    vec3 lightDir = normalize(light.position.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.data.z);
    // attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance +
        light.attenuation.z * (distance * distance));
    // combine results
    vec3 ambient = light.ambient.xyz * vec3(material.data.x);
    vec3 diffuse = light.diffuse.xyz * diff * vec3(material.data.x);
    vec3 specular = light.specular.xyz * spec * vec3(material.data.y);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    if(length(light.ambient) + length(light.diffuse) + length(light.specular) <= 0)
        return vec3(0);

    vec3 lightDir = normalize(light.position.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.data.z);
    // attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction.xyz));
    float epsilon = light.cutoff.x - light.cutoff.y;
    float intensity = clamp((theta - light.cutoff.y) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient.xyz * vec3(material.data.x);
    vec3 diffuse = light.diffuse.xyz * diff * vec3(material.data.x);
    vec3 specular = light.specular.xyz * spec * vec3(material.data.y);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}