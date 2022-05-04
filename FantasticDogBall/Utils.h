#pragma once
#include <string>
#include <stdexcept>
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <BulletDynamics/Dynamics/btRigidBody.h>

class Utils
{
public:
	static std::string readFile(const char* path);
	static GLenum checkError_(const char* file, int line);
	static std::string getISOCurrentTimestamp();
	static inline constexpr unsigned int str2int(const char* str, int h = 0)
	{
		return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
	}

	static void strToLower(std::string);
	static inline glm::mat3 toGLM(btMatrix3x3 mat) {
		return glm::mat3(
			mat[0][0], mat[0][1], mat[0][2],
			mat[1][0], mat[1][1], mat[1][2],
			mat[2][0], mat[2][1], mat[2][2]
		);
	}

	static inline btMatrix3x3 toBT(glm::mat3 mat) {
		return btMatrix3x3(
			mat[0][0], mat[0][1], mat[0][2],
			mat[1][0], mat[1][1], mat[1][2],
			mat[2][0], mat[2][1], mat[2][2]
		);
	}

	template<typename ... Args>
	static std::string string_format(const std::string& format, Args ... args)
	{
		int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
		if (size_s <= 0) { throw std::logic_error("Error during formatting."); }
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
