#include "Texture.h"

#include <stdexcept>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Utils.h"

Texture::Texture(std::string filePath_)
{
	if (filePath_.empty())
	{
		defined = false;
		substituteValue = 0.0f;
	}
	else
	{
		filePath = filePath_;
		data = stbi_load(filePath_.c_str(), &width, &height, &nrChannels, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &glID);

		glTextureParameteri(glID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(glID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(glID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(glID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

Texture::Texture(float substituteValue_) : defined(false), substituteValue(substituteValue_), nrChannels(0), width(0), height(0), data(nullptr)
{
}

void Texture::bind(unsigned location) const
{
	glBindTextureUnit(location, glID);
	Utils::checkError();
}

Texture::Cubemap::Cubemap(std::vector<std::string> paths)
{
	
}
