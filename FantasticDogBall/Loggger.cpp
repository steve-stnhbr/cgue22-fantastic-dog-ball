#include "Loggger.h"

#include "Utils.h"

Loggger::Level Loggger::LOG_LEVEL = Loggger::Level::DEBUG;

const char* colorMap[6] = {
	"37",	//TRACE: white
	"36",	//DEBUG: cyan
	"95",	//INFO: bright magenta
	"33",	//WARN: yellow
	"91",	//ERROR: bright red
	"31"		//FATAL: red
};

const char* stringMap[6] = {
	"TRACE",
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR",
	"FATAL"
};

void Loggger::log(Level level, const std::string& message)
{
	if (level >= Loggger::LOG_LEVEL)
		constructMessage(level, message);
}

void Loggger::trace(const std::string& message)
{
	log(Loggger::TRACE, message);
}

void Loggger::debug(const std::string& message)
{
	log(Loggger::DEBUG, message);
}

void Loggger::info(const std::string& message)
{
	log(Loggger::INFO, message);
}

void Loggger::warn(const std::string& message)
{
	log(Loggger::WARN, message);
}

void Loggger::error(const std::string& message)
{
	log(Loggger::ERROR, message);
}

void Loggger::fatal(const std::string& message)
{
	log(Loggger::FATAL, message);
}



template<typename ... Args>
void Loggger::log(Level level, const std::string& message, Args ... args)
{
	if (level >= Loggger::LOG_LEVEL)
		constructMessage(level, message, Utils::string_format(message, args));
}

template<typename ... Args>
void Loggger::trace(const std::string& message, Args ... args)
{
	log(Loggger::TRACE, message, args);
}

template<typename ... Args>
void Loggger::debug(const std::string& message, Args ... args)
{
	log(Loggger::DEBUG, message, args);
}

template<typename ... Args>
void Loggger::info(const std::string& message, Args ... args)
{
	log(Loggger::INFO, message, args);
}

template<typename ... Args>
void Loggger::warn(const std::string& message, Args ... args)
{
	log(Loggger::WARN, message, args);
}

template<typename ... Args>
void Loggger::error(const std::string& message, Args ... args)
{
	log(Loggger::ERROR, message, args);
}

template<typename ... Args>
void Loggger::fatal(const std::string& message, Args ... args)
{
	log(Loggger::FATAL, message, args);
}


void Loggger::setLevel(const Loggger::Level level)
{
	Loggger::LOG_LEVEL = level;
}

const char* color(Loggger::Level level)
{
	return colorMap[level];
}
const char* name(Loggger::Level level)
{
	return stringMap[level];
}

const char* timestamp()
{
	return Utils::getISOCurrentTimestamp().c_str();
}

void Loggger::constructMessage(const Level level, const std::string& msg)
{
	fprintf(stdout, "\x1B[%sm[%s]: (%s) %s\033[0m\n", name(level), timestamp(), color(level), msg.c_str());
}
