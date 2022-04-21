#pragma once
class UncheckedUniformBuffer
{
public:
	unsigned id = 0;
	void create();
	void create(unsigned);
	void update(unsigned, void*);
	void bind();
	void bind(unsigned);

private:
	static constexpr unsigned STANDARD_BUFFER_SIZE = 1024;
	void* data;

	void checkCreated_(const char* file, int line);

#define checkCreated checkCreated_(__FILE__, __LINE__);
};

