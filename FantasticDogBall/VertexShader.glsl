#version 460 core


layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
//layout (location = 2) in vec4 vecColor;
layout (location = 2) in vec2 iTexCoords;

layout(std140) uniform CameraData{
	mat4 model;
	mat4 view;
	mat4 projection;
};

out vec4 fragColor;
out vec3 fragPos;
out vec3 normal;
out vec2 texCoords;

void main() {
	mat4 mvp = projection * view * model;

	fragPos = vec3(model * vec4(iPos, 1.0));
	normal = mat3(transpose(inverse(model))) * iNormal;
	texCoords = iTexCoords;

	//gl_Position = vec4(position, 1.0);
	gl_Position = mvp * vec4(iPos, 1);
	fragColor = vec4(0, 1, 0, 1);
}