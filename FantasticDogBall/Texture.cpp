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
	}
	else
	{
		defined = true;
		int comp;
		filePath = filePath_;
		data = stbi_load(filePath_.c_str(), &width, &height, &nrChannels, STBI_rgb);

		if(data == nullptr)
		{
			Loggger::error("failed to load image %s\nReason: %s", filePath.c_str(), stbi_failure_reason());
			defined = false;
			substituteValue = .8f;
			/*
			unsigned char data0 = 0.8;
			data = &data0;
			width = 1;
			height = 1;
			*/
			//throw std::runtime_error(Utils::string_format("Failed to load image %s", filePath.c_str()));
		}

		Loggger::debug("Read image %s (%u w, %u h) %u channels", filePath.c_str(), width, height, nrChannels);
		
		glCreateTextures(GL_TEXTURE_2D, 1, &glID);
		Utils::checkError();
		glTextureStorage2D(glID, 1, GL_RGB8, width, height); 
		glTextureSubImage2D(glID, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		Utils::checkError();

		glTextureParameteri(glID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(glID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTextureParameteri(glID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTextureParameteri(glID, GL_TEXTURE_BASE_LEVEL, 0);
		glTextureParameteri(glID, GL_TEXTURE_MAX_LEVEL, 6);
		glGenerateTextureMipmap(glID);
		Utils::checkError();

		stbi_image_free(data);
	}
}

Texture::Texture::Texture(float substituteValue_) : defined(false), substituteValue(substituteValue_), nrChannels(0), width(0), height(0), data(nullptr)
{
}

void Texture::Texture::bind(unsigned location) const
{
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

