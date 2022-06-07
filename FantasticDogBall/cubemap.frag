#version 460 core

in vec3 TexCoords;

uniform samplerCube cubemap;
uniform int s_cubemap;
uniform float value_cubemap;
out vec4 color;

void main() {
	color = texture(cubemap, TexCoords);
}