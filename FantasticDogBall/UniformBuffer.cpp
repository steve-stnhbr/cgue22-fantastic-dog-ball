#include "UniformBuffer.h"


void UniformBuffer::create()
{
	create(STANDARD_BUFFER_SIZE);
}

void UniformBuffer::create(unsigned size)
{
	glCreateBuffers(size, &id);
	glNamedBufferData(id, size, nullptr, GL_DYNAMIC_DRAW);
}


void UniformBuffer::update(unsigned size, void* data)
{
	glNamedBufferSubData(id, 0, size, data);
}


void UniformBuffer::bind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, id);
}
