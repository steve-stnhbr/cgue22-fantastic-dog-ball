#include "Loggger.h"

#include "Utils.h"

Loggger::Level Loggger::LOG_LEVEL = Loggger::Level::TRACE;

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

void Loggger::setLevel(const Loggger::Level level)
{
	Loggger::LOG_LEVEL = level;
}

const char* Loggger::color(Loggger::Level level)
{
	return colorMap[level];
}
const char* Loggger::name(Loggger::Level level)
{
	return stringMap[level];
}

const char* Loggger::timestamp()
{
	return Loggger::getISOCurrentTimestamp().c_str();
}

void Loggger::constructMessage(const Level level, const std::string& msg)
{
	fprintf(stdout, "\x1B[%sm[%s]: (%s) %s\033[0m\n", color(level), name(level), timestamp(), msg.c_str());
}
