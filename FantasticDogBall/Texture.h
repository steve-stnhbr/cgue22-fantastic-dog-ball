#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>

namespace Texture
{

	class Texture
	{
	public:
		std::string filePath;
		int width, height, nrChannels;

		unsigned glID;

		float substituteValue;
		unsigned char* data;
		bool defined;

		Texture();
		Texture(std::string filePath_);
		Texture(float substituteValue_);
		Texture(unsigned width, unsigned height, GLenum colorFormat, GLenum internalFormat, GLenum type, unsigned levels, bool dsa = true);

		~Texture();

		void bind(unsigned location) const;
	private:
		void createTexture(unsigned width_, unsigned height_, GLenum colorFormat, GLenum internalFormat, GLenum type, unsigned mipmapLevels, bool dsa = true);
	};
}