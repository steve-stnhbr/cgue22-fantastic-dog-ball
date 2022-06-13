#include "Utils.h"

#include <string>
#include <fstream>
#include <GL/glew.h>

#include <algorithm>
#include <iostream>
#include <vector>
#include <filesystem>
#include "Loggger.h"

unsigned Globals::NUM_DIRECTIONAL_LIGHTS = 0;
unsigned Globals::NUM_POINT_LIGHTS = 0;
unsigned Globals::NUM_SPOT_LIGHTS = 0;
unsigned Globals::NUM_SHADOW_CUBEMAPS = 0;
unsigned Globals::NUM_SHADOW_MAPS = 0;
unsigned Globals::WINDOW_WIDTH = 1920;
unsigned Globals::WINDOW_HEIGHT = 1080;
unsigned Globals::NUM_LEVELS = 1;

std::string Utils::readFile(const char* path)
{

    std::string content;
    std::ifstream fileStream(path, std::ios::in);

    if (!fileStream.is_open()) {
        throw std::exception(string_format("could not read file %s", path).c_str());
        // Loggger::fatal("Could not read file %s", path);
        return "";
    }

    std::string line;
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLenum Utils::checkError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
		#ifdef FDB_DEBUG
			Loggger::error("%s: %s (%d)\n", error.c_str(), file, line);
		#endif
    }
    return errorCode;
}

void Utils::start2D()
{
    glUseProgram(0);
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT);
}

void Utils::end2D()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

char asciitolower(char in) {
    if (in <= 'Z' && in >= 'A')
        return in - ('Z' - 'z');
    return in;
}

void Utils::strToLower(std::string data)
{
    std::transform(data.begin(), data.end(), data.begin(), asciitolower);
}

void Utils::CheckDebugLog()
{
    unsigned int count = 10; // max. num. of messages that will be read from the log
    int bufsize = 2048;
    unsigned int* sources = new unsigned int[count];
    unsigned int* types = new unsigned int[count];
    unsigned int* ids = new unsigned int[count];
    unsigned int* severities = new unsigned int[count];
    int* lengths = new int[count];
    char* messageLog = new char[bufsize];

    unsigned int retVal = glGetDebugMessageLogARB(count, bufsize, sources, types, ids,
        severities, lengths, messageLog);

    if (retVal > 0)
    {
        unsigned int pos = 0;
        for (unsigned int i = 0; i < retVal; i++)
        {
            DebugOutputToFile(sources[i], types[i], ids[i], severities[i],
            &messageLog[pos]);
            pos += lengths[i];
        }
    }

    delete[] sources;
    delete[] types;
    delete[] ids;
    delete[] severities;
    delete[] lengths;
    delete[] messageLog;
}

void Utils::DebugOutputToFile(unsigned int source, unsigned int type, unsigned int id,
    unsigned int severity, const char* message)
{
    FILE* f;
    fopen_s(&f, "Debug.txt", "a");

    if (f)
    {
        char debSource[16], debType[20], debSev[5];
        if (source == GL_DEBUG_SOURCE_API_ARB)
            strcpy_s(debSource, "OpenGL");
        else if (source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)
            strcpy_s(debSource, "Windows");
        else if (source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)
            strcpy_s(debSource, "Shader Compiler");
        else if (source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)
            strcpy_s(debSource, "Third Party");
        else if (source == GL_DEBUG_SOURCE_APPLICATION_ARB)
            strcpy_s(debSource, "Application");
        else if (source == GL_DEBUG_SOURCE_OTHER_ARB)
            strcpy_s(debSource, "Other");
        
        if (type == GL_DEBUG_TYPE_ERROR_ARB)
            strcpy_s(debType, "Error");
        else if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)
            strcpy_s(debType, "Deprecated behavior");
        else if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
            strcpy_s(debType, "Undefined behavior");
        else if (type == GL_DEBUG_TYPE_PORTABILITY_ARB)
            strcpy_s(debType, "Portability");
        else if (type == GL_DEBUG_TYPE_PERFORMANCE_ARB)
            strcpy_s(debType, "Performance");
        else if (type == GL_DEBUG_TYPE_OTHER_ARB)
            strcpy_s(debType, "Other");

        if (severity == GL_DEBUG_SEVERITY_HIGH_ARB)
            strcpy_s(debSev, "High");
        else if (severity == GL_DEBUG_SEVERITY_MEDIUM_ARB)
            strcpy_s(debSev, "Medium");
        else if (severity == GL_DEBUG_SEVERITY_LOW_ARB)
            strcpy_s(debSev, "Low");

        Loggger::debug("Source:%s\tType:%s\tID:%d\tSeverity:%s\tMessage:%s\n",
            debSource, debType, id, debSev, message);
        fprintf(f, "Source:%s\tType:%s\tID:%d\tSeverity:%s\tMessage:%s\n",
            debSource, debType, id, debSev, message);
        fclose(f);
    }

}