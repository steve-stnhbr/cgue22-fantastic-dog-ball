#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
class UncheckedUniformBuffer
{
public:
	unsigned id = 0;
	void create();
	void create(unsigned);
	void create(unsigned size, GLenum use);
	void update(void*);
	void update(unsigned, void*);
	void bind();
	void bind(unsigned);

private:
	static constexpr unsigned STANDARD_BUFFER_SIZE = 1024;
	unsigned size;
	void* data;

	void checkCreated_(const char* file, int line) const;

#define checkCreated checkCreated_(__FILE__, __LINE__);
};

