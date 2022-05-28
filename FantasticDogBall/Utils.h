#pragma once
#include <string>
#include <GL/glew.h>
#include<iterator>
#include<algorithm>

#include "Shaders.h"

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

	template<typename V>
	static int getIndex(std::vector<V> v, V el) {
		auto it = find(v.begin(), v.end(), el);
		if (it != v.end())
		{
			int index = it - v.begin();
			return index;
		}
		else {
			return -1;
		}
	}

	template<typename V>
	static bool addUnique(std::vector<V>& v, V el) {
		if (getIndex(v, el) == -1) {
			v.push_back(el);
			return true;
		}
		return false;
	}

	template<typename V>
	static bool addAllUnique(std::vector<V>& v, const std::vector<V>& elements) {
		bool added = false;
		for (V el : elements)
			added = addUnique(v, el) || added;
		return added;
	}

	template<typename V>
	static bool remove(std::vector<V>& v, const int index) {
		if (index < 0 || index >= v.size()) {
			Loggger::warn("Index %i is out of range", index);
			return false;
		}

		v.erase(v.begin() + index);

		return true;
	}

	template<typename V>
	static bool remove(std::vector<V>& v, const V el) {
		int index = getIndex(v, el);
		return remove(v, index);
	}

	template<typename V>
	static std::vector<std::vector<V>> splitNWays(std::vector<V> v, const unsigned n) {
		std::vector<std::vector<V>> splits;
		splits.reserve(n);

		unsigned chunkSize = floor(v.size() / n);
		auto begin = v.begin();
		for (auto i = 0; i < n; i++) {
			splits.push_back(std::vector<V>(v.begin() + (i * chunkSize), v.begin() + (i + 1) * chunkSize));
		}

		if (v.size() % n != 0)
			splits[n - 1].push_back(v[v.size() - 1]);

		return splits;
	}

	template <typename V>
	static bool contains(std::vector<V> v, V el) {
		return std::count(v.begin(), v.end(), el);
	}

	static void CheckDebugLog();
	static void DebugOutputToFile(unsigned int source, unsigned int type, unsigned int id,
		unsigned int severity, const char* message);
	static int getLogicalCores();


	#define checkError() checkError_(__FILE__, __LINE__) 
};
