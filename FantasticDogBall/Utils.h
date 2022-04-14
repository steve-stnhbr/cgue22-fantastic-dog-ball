#pragma once
#include <string>
#include <GL/glew.h>

class Utils
{
public:
	static std::string readFile(const char* path);
	static GLenum checkError_(const char* file, int line);


	#define checkError() checkError_(__FILE__, __LINE__) 
};
