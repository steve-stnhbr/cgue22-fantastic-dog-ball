#version 460 core

const int NUM_LIGHTS = 1;

struct DirLight {
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    
    bool castsShadow;
    mat4 lightSpace;
    sampler2D shadowMap;
};

struct PointLight {
    vec4 position;
    vec4 attenuation;   // 0: constant, 1: linear, 2: quadratic
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    
    bool castsShadow;
    mat4 lightSpace;
    samplerCubeShadow shadowMap;
};
 
struct SpotLight {
    vec4 position;
    vec4 direction;
    vec4 cutoff;        // 0: cutoff, 1: outerCutoff
    vec4 attenuation;   // 0: constant, 1: linear, 2: quadratic
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    
    bool castsShadow;
    mat4 lightSpace;
    sampler2DShadow shadowMap;
};

uniform PointLight pLights[NUM_POINT_LIGHTS]; // error can be ignored since the value is inserted at runtime

uniform DirLight dLights[NUM_DIRECTIONAL_LIGHTS]; // error can be ignored since the value is inserted at runtime

uniform SpotLight sLights[NUM_SPOT_LIGHTS]; // error can be ignored since the value is inserted at runtime

layout(std140) uniform CameraData {
    mat4 model;
    mat4 view;
    mat4 projection;
    vec4 viewPos;
};

layout(std140) uniform Material {
    vec4 color;
    vec4 data;          //0: diffuse, 1: specular, 2: shininess
} material;

uniform samplerCube cubemap;
uniform int s_cubemap;
uniform float value_cubemap;

in vec4 fragColor;
in vec3 fragPos;
in vec3 fragNormal;
in vec2 texCoords;

out vec4 outColor;

vec3 CubemapReflection(vec3 normal, vec3 directionToCam);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float ShadowCalculationDir(vec4 fragPosLightSpace, sampler2D shadowMap, vec3 lightDir);

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
    for (int i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++) { // error can be ignored since the value is inserted at runtime
        float shadow = dLights[i].castsShadow ? ShadowCalculationDir(dLights[i].lightSpace * vec4(fragPos, 1), dLights[i].shadowMap, vec3(-dLights[i].direction)) : 0;
        //float shadow = dLights[i].castsShadow ? ShadowCalculation(dLights[i].lightSpace * vec4(fragPos, 1), dLights[i].shadowMap) : 0;
        result += CalcDirLight(dLights[i], norm, viewDir, shadow);
    }
    // phase 2: point lights
    for (int i = 0; i < NUM_POINT_LIGHTS; i++) { // error can be ignored since the value is inserted at runtime
        result += CalcPointLight(pLights[i], norm, fragPos, viewDir);
    }
    // phase 3: spot light
    for (int i = 0; i < NUM_SPOT_LIGHTS; i++) { // error can be ignored since the value is inserted at runtime 
        result += CalcSpotLight(sLights[i], norm, fragPos, viewDir);
    }

    result += s_cubemap * CubemapReflection(norm, viewDir) * material.data[3];

    outColor = vec4(result * material.color.xyz, material.color.a);
}

float ShadowCalculationDir(vec4 fragPosLightSpace, sampler2D shadowMap, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fragNormal);
    //vec3 lightDir = normalize(lightPos - fragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}
vec3 CubemapReflection(vec3 normal, vec3 directionToCam) {
	vec3 reflectDir = reflect(directionToCam, normal);
	return vec3(texture(cubemap, -reflectDir));
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow)
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
    return (ambient + (1.0 - shadow) * (diffuse + specular));
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