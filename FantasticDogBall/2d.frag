#version 460 core

in vec2 texCoord;

uniform sampler2D hudImage;
uniform int s_hudImage;
uniform float value_hudImage;

out vec4 color;

void main() {
	color = texture(hudImage, texCoord);
}