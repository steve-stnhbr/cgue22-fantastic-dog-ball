#version 460 core

layout (location = 0) in vec3 position;

uniform mat4 lightSpace;

void main()
{
    gl_Position = lightSpace * vec4(position, 1.0);
}