#version 460 core

layout (location = 0) out vec3 color;

const vec2 pos[3] = vec2[3] (
	vec2(-0.6, -0.4),
	vec2(0.6, -0.4),
	vec2(0.0, 0.6)
);

const vec3 col[3] = vec3[3] (
	vec3(1.0, 0.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, 0.0, 1.0)
);

void main() {
	gl_Position = vec4(pos[gl_VertexID], 0.0, 1.0);
	color = col[gl_VertexID];
}

/*#version 460 core

layout(std140, binding = 0) uniform TestBufferData {
	uniform mat4 MVP;
	uniform int isWireframe;
};

layout (location = 0) out vec3 outColor;

const vec3 pos[8] = vec3[8] (
	vec3(-1.0,	-1.0,	1.0), vec3(1.0,		-1.0,	1.0),
	vec3(-1.0,	-1.0,	1.0), vec3(-1.0,	1.0,	1.0),

	vec3(-1.0,	-1.0,	-1.0), vec3(1.0,	-1.0,	-1.0),
	vec3(1.0,	1.0,	-1.0), vec3(-1.0,	1.0,	-1.0)
);


const vec3 col[8] = vec3[8] (
	vec3(1.0,	0.0,	0.0), vec3(0.0,		1.0,	0.0),
	vec3(0.0,	0.0,	1.0), vec3(1.0,		1.0,	0.0),

	vec3(1.0,	1.0,	0.0), vec3(0.0,		0.0,	1.0),
	vec3(0.0,	1.0,	0.0), vec3(1.0,		0.0,	0.0)
);

const int indices[36] = int[36] (
	0,1,2,2,3,0, //front
	1,5,6,6,2,1, //right	
	7,6,5,5,4,7, //back
	4,0,3,3,7,4, //left
	4,5,1,1,0,4, //bottom
	3,2,6,6,7,3 //top
);

void main() {
	int idx = indices[gl_VertexID];
	gl_Position = MVP * vec4(pos[idx], 1.0);
	outColor = isWireframe > 0 ? vec3(0.0) : col[idx];
}*/