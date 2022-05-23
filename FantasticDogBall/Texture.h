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


	static class Cubemap : public Texture
	{
		/*
		 * Stores all the textures for the cube in the following order:
		 * +X, -X, +Y, -Y, +Z, -Z
		 */
		std::vector<unsigned char*> textures;
		unsigned glID;


	private:
		void initGL();

	public:
		explicit Cubemap(std::string& path);
		Cubemap(std::vector<std::string> paths);
	};

	struct Coordinate
	{
		int x, y;
	};

	struct Bounds
	{
		Coordinate topLeft, bottomRight;

		bool inside(int x, int y);
	};

	unsigned char* crop(unsigned char* data_,
		int width_,
		int height_,
		Bounds bounds);

	std::vector<unsigned char*> loadCubemap(unsigned char* data_,
		int width_,
		int height_);
}
