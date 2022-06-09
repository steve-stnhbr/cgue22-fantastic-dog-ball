#pragma once

#include "Texture.h"
#include "Shaders.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#define GLT_IMPLEMENTATION
#include <gltext.h>

class HUD
{
protected:
	static Shaders::Program hudProgram;

	class Clock {
		GLTtext* glText;
		std::string text;
		float centerX, halfWidth;
		Texture::Texture* texture;
		glm::mat4 projection;

		unsigned vaoID, vboID;
	public:
		void init();
		void draw(std::string time);
	} clock;

	class Bones {
		GLTtext* glText;
		std::string text;
		Texture::Texture* texture;
		glm::mat4 projection;

		float size, margin;

		unsigned vaoID, vboID;
	public:
		void init();
		void draw(std::string amount);
	} bones;

public:

	HUD();

	void init();
	void draw(std::string time, std::string bones);
	void cleanup();
};

