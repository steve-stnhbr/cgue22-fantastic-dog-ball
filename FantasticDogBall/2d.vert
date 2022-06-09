#version 460 core

layout (location = 0) in vec2 iPosition;
layout (location = 1) in vec2 iTexCoord;

uniform mat4 projection;

out vec2 texCoord;

void main() {
	texCoord = iTexCoord;
	vec4 pos = projection * vec4(iPosition, 1, 1);
	gl_Position = pos;
}