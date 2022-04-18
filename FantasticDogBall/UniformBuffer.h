#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class UniformBuffer
{
public:
	unsigned id = 0;
	void create();
	void create(unsigned);
	void update(unsigned, void*);
	void bind();
	void bind(unsigned);

private:
	static const unsigned STANDARD_BUFFER_SIZE = 1024;
	void* data;

	void checkCreated_(const char* file, int line);

#define checkCreated checkCreated_(__FILE__, __LINE__);

};

