#version 460 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
//layout (location = 2) in vec4 vecColor;
layout (location = 2) in vec2 iTexCoords;

layout(std140) uniform CameraData{
	mat4 model;
	mat4 view;
	mat4 projection;
	vec4 viewPos;
};

out vec4 fragColor;
out vec3 fragPos;
out vec3 fragNormal;
out vec2 texCoords;

void main() {
	mat4 mvp = projection * view * model;

	fragColor = view[1];
	fragPos = vec3(model * vec4(iPos, 1.0));
	fragNormal = mat3(transpose(inverse(model))) * iNormal;
	texCoords = iTexCoords;

	gl_Position = mvp * vec4(iPos, 1);
}