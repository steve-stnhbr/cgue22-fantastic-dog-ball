#pragma once
#include <string>

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
	static void trace(const std::string&);
	static void debug(const std::string&);
	static void info(const std::string&);
	static void warn(const std::string&);
	static void error(const std::string&);
	static void fatal(const std::string&);

	template<typename ... Args>
	static void log(Level, const std::string&, Args ... args);
	template<typename ... Args>
	static void trace(const std::string&, Args ... args);
	template<typename ... Args>
	static void debug(const std::string&, Args ... args);
	template<typename ... Args>
	static void info(const std::string&, Args ... args);
	template<typename ... Args>
	static void warn(const std::string&, Args ... args);
	template<typename ... Args>
	static void error(const std::string&, Args ... args);
	template<typename ... Args>
	static void fatal(const std::string&, Args ... args);

	static void setLevel(const Level);

private:
	static void constructMessage(const Level level, const std::string&);
};

