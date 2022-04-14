#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class UniformBuffer
{
public:
	void create();
	void create(unsigned);
	void update(unsigned, void*);
	void bind();

private:
	const unsigned STANDARD_BUFFER_SIZE = 1024;
	unsigned id;
	void* data;

};

