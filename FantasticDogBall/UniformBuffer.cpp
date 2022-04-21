#include "UniformBuffer.h"

#include "Loggger.h"
#include "Utils.h"



template<typename T>
UniformBuffer<T>::UniformBuffer()
{
	create(sizeof(T));
}

template <typename T>
void UniformBuffer<T>::create()
{
	create(STANDARD_BUFFER_SIZE);
}

template <typename T>
void UniformBuffer<T>::create(const unsigned size)
{
	glCreateBuffers(1, &id);
	Utils::checkError();
	glNamedBufferData(id, size, nullptr, GL_STATIC_DRAW);
	Utils::checkError();
}


template <typename T>
void UniformBuffer<T>::update(unsigned size, T* data_)
{
	data = data_;
	checkCreated
	glNamedBufferSubData(id, 0, size, data);
	Utils::checkError();
}


template <typename T>
void UniformBuffer<T>::update(T* data_)
{
	data = data_;
	checkCreated
		glNamedBufferSubData(id, 0, sizeof(T), data);
	Utils::checkError();
}


template <typename T>
void UniformBuffer<T>::bind()
{
	checkCreated
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	Utils::checkError();
}


template <typename T>
void UniformBuffer<T>::bind(unsigned binding)
{
	checkCreated
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, id);
	Loggger::debug("binding buffer " + std::to_string(id) + " to binding " + std::to_string(binding));
	Utils::checkError();
}

template <typename T>
void UniformBuffer<T>::checkCreated_(const char* file, int line)
{
	if (id == 0)
		throw std::exception("Buffer was not created yet");
}
