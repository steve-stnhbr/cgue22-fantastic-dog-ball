#version 460 core

layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

layout(std140) uniform CameraData {
    mat4 model;
    mat4 view;
    mat4 projection;
    vec4 viewPos;
};

void main() {
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}