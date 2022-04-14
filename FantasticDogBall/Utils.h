#pragma once
#include <string>
#include <GL/glew.h>

class Utils
{
public:
	static std::string readFile(const char* path);
	template<typename ... Args>
	static std::string string_format(const std::string& format, Args ... args);
	static GLenum checkError_(const char* file, int line);


	#define checkError() checkError_(__FILE__, __LINE__) 
};
