#include "UniformBuffer.h"

#include "Utils.h"

void UniformBuffer::create()
{
	create(STANDARD_BUFFER_SIZE);
}

void UniformBuffer::create(unsigned size)
{
	glCreateBuffers(size, &id);
	Utils::checkError();
	glNamedBufferData(id, size, nullptr, GL_DYNAMIC_DRAW);
	Utils::checkError();
}


void UniformBuffer::update(unsigned size, void* data_)
{
	data = data_;
	checkCreated();
	glNamedBufferSubData(id, 0, size, data);
	Utils::checkError();
}


void UniformBuffer::bind()
{
	checkCreated();
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	Utils::checkError();
}

void UniformBuffer::checkCreated_(const char* file, int line)
{
	if (id == 0)
		throw std::exception("Buffer was not created yet");
}
