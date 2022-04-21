#include "UncheckedUniformBuffer.h"

#include "Loggger.h"
#include "Utils.h"

void UncheckedUniformBuffer::create()
{
	size = STANDARD_BUFFER_SIZE;
	create(size);
}

void UncheckedUniformBuffer::create(const unsigned size_)
{
	size = size_;
	glCreateBuffers(1, &id);
	Utils::checkError();
	glNamedBufferData(id, size, nullptr, GL_STATIC_DRAW);
	Utils::checkError();
}

void UncheckedUniformBuffer::update(void* data_)
{
	update(size, data_);
}


void UncheckedUniformBuffer::update(unsigned size_, void* data_)
{
	data = data_;
	checkCreated
	glNamedBufferSubData(id, 0, size_, data);
	Utils::checkError();
}


void UncheckedUniformBuffer::bind()
{
	checkCreated
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	Utils::checkError();
}


void UncheckedUniformBuffer::bind(unsigned binding)
{
	checkCreated
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, id);
	Loggger::debug("binding buffer " + std::to_string(id) + " to binding " + std::to_string(binding));
	Utils::checkError();
}

void UncheckedUniformBuffer::checkCreated_(const char* file, int line) const
{
	if (id == 0)
		throw std::exception("Buffer was not created yet");
}
