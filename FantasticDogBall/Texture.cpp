#include "Texture.h"

#include <stdexcept>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_image_resize.h>

#include "Utils.h"
#include "Loggger.h"

Texture::Texture::Texture() : Texture("")
{
}


Texture::Texture::Texture(std::string filePath_)
{
	if (filePath_.empty())
	{
		defined = false;
		substituteValue = 0.8f;
		Loggger::info("Setting substitute value");
	}
	else
	{
		load(filePath_);
	}
}

Texture::Texture::Texture(float substituteValue_) : glID(0), defined(false), substituteValue(substituteValue_), nrChannels(0), width(0), height(0), data(nullptr)
{
	Loggger::info("Setting substitute value");
}

Texture::Texture::Texture(unsigned width_, unsigned height_, GLenum internalFormat, GLenum colorFormat, GLenum type, unsigned mipmapLevels, bool dsa): width(width_), height(height_), defined(true), data(nullptr)
{
	createTexture(width_, height_, internalFormat, colorFormat, type, mipmapLevels, dsa);
}

Texture::Texture::~Texture()
{
	Loggger::trace("Deleting Texture %l", glID);
	//glDeleteTextures(1, &glID);
}

void Texture::Texture::createTexture(unsigned width_, unsigned height_, GLenum internalFormat, GLenum colorFormat, GLenum type, unsigned mipmapLevels, bool dsa)
{
	if (dsa) {
		glCreateTextures(GL_TEXTURE_2D, 1, &glID);
		Utils::checkError();
		glTextureStorage2D(glID, mipmapLevels, internalFormat, width_, height_);
		Utils::checkError();
		glTextureSubImage2D(glID, 0, 0, 0, width_, height_, colorFormat, type, data);
		Utils::checkError();

		glTextureParameteri(glID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(glID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTextureParameteri(glID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glGenerateTextureMipmap(glID);
		Utils::checkError();
	}
	else {
		glGenTextures(1, &glID);
		glBindTexture(GL_TEXTURE_2D, glID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width_, height_, 0, colorFormat, type, 0);
		Utils::checkError();
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, colorFormat, type, data);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmapLevels - 1);
		glGenerateMipmap(GL_TEXTURE_2D);
		Utils::checkError();
	}
}

void Texture::Texture::load(std::string filePath_)
{
	defined = true;
	int comp;
	filePath = filePath_;
	data = stbi_load(filePath_.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
	stbi_set_flip_vertically_on_load(true);

	if (data == nullptr)
	{
		Loggger::error("failed to load image %s\nReason: %s", filePath.c_str(), stbi_failure_reason());
		defined = false;
		substituteValue = .8f;

		Loggger::error("Setting substitute value because of an error");
	}

	Loggger::debug("Read image %s (%u w, %u h) %u channels", filePath.c_str(), width, height, nrChannels, 4);

	createTexture(width, height, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 3);

	stbi_image_free(data);
}

void Texture::Texture::bind(unsigned location) const
{
	if (glID == 0) {
		Loggger::error("Texture to be bound to %u is not defined", location);
	}
	Loggger::debug("binding texture %u to location %u", glID, location);
	glBindTextureUnit(location, glID);
	Utils::checkError();
}


