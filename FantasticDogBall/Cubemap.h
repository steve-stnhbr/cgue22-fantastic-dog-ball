#pragma once

#include <vector>

#include "Camera.h"
#include "Loggger.h"
#include "RenderObject.h"

class Cubemap : public Texture::Texture
{
	/*
	 * Stores all the textures for the cube in the following order:
	 * +X, -X, +Y, -Y, +Z, -Z
	 */
	std::vector<unsigned char*> textures;
	unsigned glID;

private:
	static Shaders::Program cubemapProgram;
	static RenderObject* skybox;
	static float skyboxVertices[];
	static unsigned int skyboxVAO, skyboxVBO;
	void initGL();
	std::vector<unsigned char*> loadPaths(std::vector<std::string>);

public:
	Cubemap(float substitute);
	Cubemap(std::string path);
	Cubemap(std::vector<std::string> paths);

	void draw(Camera camera);


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
};