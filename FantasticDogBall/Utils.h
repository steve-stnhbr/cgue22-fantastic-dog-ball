#pragma once
#include <string>
#include <GL/glew.h>

#include "Shaders.h"

class Globals {
public:
	static unsigned NUM_POINT_LIGHTS;
	static unsigned NUM_DIRECTIONAL_LIGHTS;
	static unsigned NUM_SPOT_LIGHTS;
	static unsigned NUM_SHADOW_CUBEMAPS;
	static unsigned NUM_SHADOW_MAPS;
	static unsigned WINDOW_WIDTH;
	static unsigned WINDOW_HEIGHT;
};

class Utils
{
public:
	static std::string readFile(const char* path);
	static GLenum checkError_(const char* file, int line);
	static Shaders::Program loadProgram(std::string, std::string);
	static std::string getISOCurrentTimestamp();
	static inline constexpr unsigned int str2int(const char* str, int h = 0)
	{
		return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
	}

	static void strToLower(std::string);

	template<typename ... Args>
	static std::string string_format(const std::string& format, Args ... args)
	{
		int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
		if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
		auto size = static_cast<size_t>(size_s);
		char* buf(new char[size]);
		std::snprintf(buf, size, format.c_str(), args ...);
		return std::string(buf, buf + size - 1); // We don't want the '\0' inside
	}

	template<typename Base, typename T>
	static bool instanceof(const T*) {
		return std::is_base_of<Base, T>::value;
	}
	static void CheckDebugLog();
	static void DebugOutputToFile(unsigned int source, unsigned int type, unsigned int id,
		unsigned int severity, const char* message);


	#define checkError() checkError_(__FILE__, __LINE__) 
};
