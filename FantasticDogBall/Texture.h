#pragma once
#include <string>
#include <vector>

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


	static class Cubemap
	{
		/*
		 * Stores all the textures for the cube in the following order:
		 * +X, -X, +Y, -Y, +Z, -Z
		 */
		std::vector<Texture> textures;

	public:
		int glID;

		Cubemap(std::vector<std::string> paths);
	};
};

