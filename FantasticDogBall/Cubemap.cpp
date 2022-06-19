#include "Cubemap.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_image_resize.h>
#include <exception>
#include <filesystem>

Shaders::Program Cubemap::cubemapProgram;
RenderObject* Cubemap::skybox;
float Cubemap::skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};
unsigned int Cubemap::skyboxVAO;
unsigned int Cubemap::skyboxVBO;


void Cubemap::initGL()
{
	glGenTextures(1, &glID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, glID);

	for (auto i = 0; i < textures.size(); i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[i]);
		stbi_image_free(textures[i]);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glCreateVertexArrays(1, &skyboxVAO);
	glCreateBuffers(1, &skyboxVBO);
	glNamedBufferStorage(skyboxVBO, sizeof(skyboxVertices), skyboxVertices, GL_CLIENT_STORAGE_BIT);
	glEnableVertexArrayAttrib(skyboxVAO, 0);
	glVertexArrayAttribFormat(skyboxVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(skyboxVAO, 0, 0);
	glVertexArrayVertexBuffer(skyboxVAO, 0, skyboxVBO, 0, sizeof(float) * 3);
	Utils::checkError();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

std::vector<unsigned char*> Cubemap::loadPaths(std::vector<std::string> paths)
{
	if (paths.size() != 6) {
		Loggger::error("Cubemap can only be initialized with 6 textures, not " + std::to_string(paths.size()));
		defined = false;
		return std::vector<unsigned char*>();
	}
	std::vector<unsigned char*> texs(6);

	for (auto i = 0; i < paths.size(); i++)
	{
		texs[i] = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
	}

	return texs;
}

Cubemap::Cubemap(float substitute)
{
	defined = false;
	substituteValue = substitute;
}

Cubemap::Cubemap(std::string path)
{
	if (path.substr(path.find_last_of("/"), std::string::npos).find(".") != std::string::npos) { // if given path is folder
		int width, height, nrChannels;

		data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data == nullptr) {
			defined = false;
			Loggger::error("Failed to load cubemap from single image");
			return;
		}
		textures = loadCubemap(data, width, height);
	}
	else {
		std::vector<std::string> paths;
		for(const std::filesystem::directory_entry& dir_entry :
			std::filesystem::directory_iterator(path.c_str()))
		{
			paths.push_back(dir_entry.path().string());
		}

		textures = loadPaths(paths);
		if (textures.size() != 6)
			return;
	}

	defined = true;

	initGL();
}

Cubemap::Cubemap(std::vector<std::string> paths)
{
	textures = loadPaths(paths);
	initGL();
}

void Cubemap::draw(Camera camera)
{
	if(cubemapProgram.ID == 0) 
		cubemapProgram = { {"cubemap.vert", "cubemap.frag"} };
	glDepthFunc(GL_LEQUAL);
	glBindTexture(GL_TEXTURE_CUBE_MAP, glID);
	cubemapProgram.use();
	camera.bindCubemap(cubemapProgram);
	glBindVertexArray(skyboxVAO);
	cubemapProgram.setTexture("cubemap", this);
	Utils::checkError();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	Utils::checkError();
}

void Cubemap::bind(unsigned location) const
{
	glActiveTexture(GL_TEXTURE0 + location);
	glBindTexture(GL_TEXTURE_CUBE_MAP, glID);
}

bool Cubemap::Bounds::inside(int x, int y)
{
	return x >= topLeft.x && y >= topLeft.y
		&& x < bottomRight.x&& y < bottomRight.y;
}

unsigned char* Cubemap::crop(unsigned char* data_,
	int width_,
	int height_,
	Bounds bounds)
{
	// x = i % height
	// y = floor(i / height)
	std::vector<unsigned char> oldPixels, newPixels;
	oldPixels.assign(data_, data_ + width_ * height_);

	for (auto i = 0; i < oldPixels.size(); i++)
	{
		const unsigned x = i % height_;
		const unsigned y = floor(i / height_);

		if (x > bounds.bottomRight.x && y > bounds.bottomRight.y)
		{
			break;
		}

		if (bounds.inside(x, y))
		{
			newPixels.push_back(oldPixels[i]);
		}
	}

	return newPixels.data();
}

std::vector<unsigned char*> Cubemap::loadCubemap(unsigned char* data_,
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

		for (auto j = 0; j < boundings.size(); j++)
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