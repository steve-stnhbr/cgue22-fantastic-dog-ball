#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 vecColor;

layout(std140) uniform CameraData{
	mat4 mvp;
};

layout (std140) uniform Material{
	vec4 matColor;
	float roughness;
	float transmission;
	float indexOfRefraction;
	float metallic;
	float specularity;
};

out vec4 fragColor;

void main() {
	//gl_Position = vec4(position, 1.0);
	gl_Position = mvp * vec4(position, 1);
	fragColor = matColor;
}