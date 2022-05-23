#include "Texture.h"

#include <stdexcept>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <stb/stb_image_resize.h>

#include "Utils.h"

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
		defined = true;
		int comp;
		filePath = filePath_;
		data = stbi_load(filePath_.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

		if(data == nullptr)
		{
			Loggger::error("failed to load image %s\nReason: %s", filePath.c_str(), stbi_failure_reason());
			defined = false;
			substituteValue = .8f;

			Loggger::error("Setting substitute value because of an error");
		} 

		Loggger::debug("Read image %s (%u w, %u h) %u channels", filePath.c_str(), width, height, nrChannels, 4);
		
		createTexture(width, height, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 6);

		stbi_image_free(data);
	}
}

Texture::Texture::Texture(float substituteValue_) : glID(0), defined(false), substituteValue(substituteValue_), nrChannels(0), width(0), height(0), data(nullptr)
{
	Loggger::info("Setting substitute value");
}

Texture::Texture::Texture(unsigned width_, unsigned height_, GLenum internalFormat, GLenum colorFormat, GLenum type, unsigned mipmapLevels): width(width_), height(height_), defined(true), data(nullptr)
{
	createTexture(width_, height_, internalFormat, colorFormat, type, mipmapLevels);
}

Texture::Texture::~Texture()
{
	Loggger::trace("Deleting Texture %l", glID);
	//glDeleteTextures(1, &glID);
}

void Texture::Texture::createTexture(unsigned width_, unsigned height_, GLenum internalFormat, GLenum colorFormat, GLenum type, unsigned mipmapLevels)
{
	/*
	glCreateTextures(GL_TEXTURE_2D, 1, &glID);
	Utils::checkError();
	glTextureStorage2D(glID, 1, internalFormat, width, height);
	Utils::checkError();
	glTextureSubImage2D(glID, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), colorFormat, type, data);
	Utils::checkError();

	glTextureParameteri(glID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(glID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTextureParameteri(glID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glGenerateTextureMipmap(glID);
	Utils::checkError();
	*/

	//todo for some reason dsa is not working with depth_components so we need to use bound configuration

	glGenTextures(1, &glID);
	glBindTexture(GL_TEXTURE_2D, glID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, colorFormat, type, NULL);
	Utils::checkError();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmapLevels - 1);
	glGenerateMipmap(GL_TEXTURE_2D);
	Utils::checkError();
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

void Texture::Cubemap::initGL()
{
	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &glID);

	auto side = GL_TEXTURE_CUBE_MAP_POSITIVE_X;

	for (const auto e : textures)
	{
		glTextureStorage2D(glID, 1, GL_RGB, width, height);
	}

	glTextureParameteri(glID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(glID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(glID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTextureParameteri(glID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(glID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTextureParameteri(glID, GL_TEXTURE_BASE_LEVEL, 1);
	glTextureParameteri(glID, GL_TEXTURE_MAX_LEVEL, 1);
}

Texture::Cubemap::Cubemap(std::string& path)
{
	int width, height, nrChannels; 

	data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data == nullptr) {
		defined = false;
		Loggger::error("Failed to load cubemap from single image");
	}
	textures = loadCubemap(data, width, height);

	initGL();
}

Texture::Cubemap::Cubemap(std::vector<std::string> paths)
{
	if (paths.size() != 6)
		throw std::runtime_error("Cubemap can only be initialized with 6 textures, not " + std::to_string(paths.size()));

	for(auto i = 0; i < paths.size(); i++)
	{
		textures[i] = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
	}

	initGL();
}

bool Texture::Bounds::inside(int x, int y)
{
	return x >= topLeft.x && y >= topLeft.y
		&& x < bottomRight.x&& y < bottomRight.y;
}

unsigned char* Texture::crop(unsigned char* data_,
                             int width_,
                             int height_, 
                             Bounds bounds)
{
	// x = i % height
	// y = floor(i / height)
	std::vector<unsigned char> oldPixels, newPixels;
	oldPixels.assign(data_, data_ + width_ * height_);

	for(auto i = 0; i < oldPixels.size(); i++)
	{
		const unsigned x = i % height_;
		const unsigned y = floor(i / height_);

		if(x > bounds.bottomRight.x && y > bounds.bottomRight.y)
		{
			break;
		}

		if(bounds.inside(x, y))
		{
			newPixels.push_back(oldPixels[i]);
		}
	}

	return newPixels.data();
}

std::vector<unsigned char*> Texture::loadCubemap(unsigned char* data_,
	int width_,
	int height_)
{
	// x = i % width
	// y = floor(i / width)
	const int tileSize = width_ / 4;

	std::vector<Bounds> boundings = {
		{{tileSize * 2, tileSize},  {tileSize * 3, tileSize * 3}},
		{{0, tileSize},  {tileSize, tileSize * 2}},
		{{tileSize, 0},  {tileSize * 2, tileSize}},
		{{tileSize, tileSize * 2},  {tileSize * 2, tileSize * 3}},
		{ {tileSize, tileSize},  {tileSize * 2, tileSize * 2}},
		{{tileSize * 3, tileSize},  {tileSize * 4, tileSize}},
	};
	std::vector<unsigned char> oldPixels;
	std::vector<std::vector<unsigned char>> newPixels;
	oldPixels.assign(data_, data_ + width_ * height_);

	for (auto i = 0; i < oldPixels.size(); i++)
	{
		const unsigned x = i % height_;
		const unsigned y = floor(i / height_);

		for(auto j = 0; j < boundings.size(); j++)
		{
			if (boundings[j].inside(x, y))
			{
				newPixels[j].push_back(oldPixels[i]);
			}
		}
	}

	std::vector<unsigned char*> ret;
	for (auto e : newPixels)
	{
		ret.push_back(e.data());
	}

	return ret;
}

