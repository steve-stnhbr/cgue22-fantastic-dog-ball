#pragma once
#include <string>
#include <time.h>
#include <stdexcept>
#include <future>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Loggger
{
public:
	enum Level
	{
		TRACE = 0,
		DEBUG = 1,
		INFO = 2,
		WARN = 3,
		ERROR = 4,
		FATAL = 5
	};

	static Level LOG_LEVEL;

	static void log(Level, const std::string&);
	static void log(GLenum, const std::string&);

	static void trace(const std::string&);
	static void debug(const std::string&);
	static void info(const std::string&);
	static void warn(const std::string&);
	static void error(const std::string&);
	static void fatal(const std::string&);

	static Level severityToLevel(GLenum);

	template<typename ... Args>
	static void log(Level level, const std::string& msg, Args ... args)
	{
		if (level >= Loggger::LOG_LEVEL)
			constructMessage(level, string_format(msg, args...));
	}
	template<typename ... Args>
	static void logAsync(Level level, const std::string& msg, Args... args) {
		if (level >= Loggger::LOG_LEVEL)
			std::async(constructMessage, level, string_format(msg, args...));
	}

	template<typename ... Args>
	static void log(GLenum severity, const std::string& msg, Args ... args)
	{
		logAsync(severityToLevel(severity), msg, args...);
	}
	template<typename ... Args>
	static void trace(const std::string& msg, Args ... args)
	{
		logAsync(TRACE, msg, args...);
	}
	template<typename ... Args>
	static void debug(const std::string& msg, Args ... args)
	{
		logAsync(DEBUG, msg, args...);
	}
	template<typename ... Args>
	static void info(const std::string& msg, Args ... args)
	{
		logAsync(INFO, msg, args...);
	}
	template<typename ... Args>
	static void warn(const std::string& msg, Args ... args)
	{
		logAsync(WARN, msg, args...);
	}
	template<typename ... Args>
	static void error(const std::string& msg, Args ... args)
	{
		logAsync(ERROR, msg, args...);
	}
	template<typename ... Args>
	static void fatal(const std::string& msg, Args ... args)
	{
		logAsync(FATAL, msg, args...);
	}

	static void setLevel(const Level);

private:
	static const char* color(Level);
	static const char* name(Level);
	static const char* timestamp();
	static void constructMessage(const Level level, const std::string& msg);

	static std::string getISOCurrentTimestamp()
	{
		return "";
	}

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
};

