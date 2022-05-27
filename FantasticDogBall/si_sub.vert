#version 460 core

uniform int levels;
uniform int size;

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
//layout (location = 2) in vec4 vecColor;
layout (location = 2) in vec2 iTexCoords;

struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	vec4 color;
};

struct Mesh {
	Vertex vertex;
	int index;
};

out Mesh mesh;

void main() {
	mesh.vertex.position = iPos;
	mesh.vertex.normal = iNormal;
	mesh.vertex.texCoord = iTexCoords;
	mesh.vertex.color = vec4(1);
}