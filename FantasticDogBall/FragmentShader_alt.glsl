#version 460 core

in vec3 viewPos;

layout(std140) uniform Material{
    vec4 matColor;
    float roughness;
    float transmission;
    float indexOfRefraction;
    float metallic;
    float shininess;
};

layout(location = 0) out vec4 fragColor;

uniform vec4 ambientMaterial2, diffuseMaterial2, specularMaterial2;
uniform vec4 ambientLight, diffuseLight, specularLight;
uniform mat4 viewMat;
uniform vec4 lightPosition;


vec3 phong(vec3 n, vec3 l, vec3 v, vec3 diffuseC, float diffuseF, vec3 specularC, float specularF, float alpha, bool attenuate, vec3 attenuation) {
    float d = length(l);
    l = normalize(l);
    float att = 1.0;
    if (attenuate) att = 1.0f / (attenuation.x + d * attenuation.y + d * d * attenuation.z);
    vec3 r = reflect(-l, n);
    return (diffuseF * diffuseC * max(0, dot(n, l)) + specularF * specularC * pow(max(0, dot(r, v)), alpha)) * att;
}


void main(void)
{
    vec4 ambientProduct = ambientLight * ambientMaterial2;
    vec4 diffuseProduct = diffuseLight * diffuseMaterial2;
    vec4 specularProduct = specularLight * specularMaterial2;

    vec3 pos = viewPos;
    vec3 nv = cross(dFdx(pos), dFdy(pos));
    nv = nv * sign(nv.z);
    vec3 L = normalize((viewMat * lightPosition).xyz - pos);
    vec3 E = normalize(-pos);
    vec3 H = normalize(L + E);
    vec3 N = normalize(nv);

    float Kd = max(dot(L, N), 0.0);
    vec4 diffuse = Kd * diffuseProduct;

    vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
    if (Kd > 0.0f)
    {
        float Ks = pow(max(dot(H, N), 1.0), shininess);
        specular = Ks * specularProduct;
    }
    vec4 fragColor_lightning = (ambientProduct + diffuse + specular);
    //vec4 fragColor_texture = texture2D(texture1, vec2(f_TexCoord0.x, f_TexCoord0.y));
    fragColor_color = matColor;
    fragColor = fragColor_lightning + fragColor_texture;
}