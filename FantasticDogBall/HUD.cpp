#include "HUD.h"

#include "Utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

Shaders::Program HUD::hudProgram;

HUD::HUD()
{
}

void HUD::init()
{
	hudProgram = { {"2d.vert", "2d.frag" } };
	gltInit();
	clock.init();
	bones.init();
}

void HUD::draw(std::string time, std::string amount) {
	clock.draw(time);
	bones.draw(amount);
}

void HUD::cleanup() {
}

void HUD::Clock::init()
{
	glText = gltCreateText();

	texture = new Texture::Texture("../res/clock.png");

	projection = glm::ortho(0.0f, (float) Globals::WINDOW_WIDTH, (float)Globals::WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);

	centerX = Globals::WINDOW_WIDTH / 2;
	halfWidth = Globals::WINDOW_WIDTH / 64 * 2;
	float leftX = centerX - halfWidth, rightX = centerX + halfWidth;
	float height = halfWidth * 2;
	
	float data[] = {
		leftX, 0, 0, 0,
		rightX, 0, 1, 0,
		rightX, height, 1, 1,
		leftX, height, 0,1,
		leftX, 0, 0, 0,
	};

	glCreateVertexArrays(1, &vaoID);
	glCreateBuffers(1, &vboID);
	glNamedBufferStorage(vboID, sizeof(data), data, GL_CLIENT_STORAGE_BIT);
	Utils::checkError();

	// position
	glEnableVertexArrayAttrib(vaoID, 0);
	glVertexArrayAttribFormat(vaoID, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoID, 0, 0);
	Utils::checkError();
	// uv coord
	glEnableVertexArrayAttrib(vaoID, 1);
	glVertexArrayAttribFormat(vaoID, 1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 2);
	glVertexArrayAttribBinding(vaoID, 1, 0);

	glVertexArrayVertexBuffer(vaoID, 0, vboID, 0, sizeof(float) * 4);
	Utils::checkError();
	glBindVertexArray(0);
}

void HUD::Clock::draw(std::string time)
{
	if (time.length() < 2) time = "0" + time;

	//draw clock
	hudProgram.use();
	hudProgram.setTexture("hudImage", *texture);
	hudProgram.setMatrix4("projection", projection);
	glBindVertexArray(vaoID);
 	glDrawArrays(GL_QUADS, 0, 5);

	gltBeginDraw();
	gltSetText(glText, time.c_str());
	gltColor(1, 1, 1, 1.0f);
	gltDrawText2D(glText, centerX - 18, 40, 2.4);
	//gltDrawText2DAligned(glText, centerX - 18, 40, 2.4, GLT_CENTER, GLT_CENTER);
	gltEndDraw();
	Utils::checkError();
}

void HUD::Bones::init()
{
	glText = gltCreateText();

	texture = new Texture::Texture("../res/bone.png");

	projection = glm::ortho(0.0f, (float)Globals::WINDOW_WIDTH, (float)Globals::WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);
	size = 70;
	margin = 14;
	const float marginSize = size + margin;
	float data[] = {
		margin, margin, 0, 0,
		marginSize, margin, 1, 0,
		marginSize, marginSize, 1, 1,
		margin, marginSize, 0, 1,
		margin, margin, 0, 0,
	};

	glCreateVertexArrays(1, &vaoID);
	glCreateBuffers(1, &vboID);
	glNamedBufferStorage(vboID, sizeof(data), data, GL_CLIENT_STORAGE_BIT);
	Utils::checkError();

	// position
	glEnableVertexArrayAttrib(vaoID, 0);
	glVertexArrayAttribFormat(vaoID, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoID, 0, 0);
	Utils::checkError();
	// uv coord
	glEnableVertexArrayAttrib(vaoID, 1);
	glVertexArrayAttribFormat(vaoID, 1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 2);
	glVertexArrayAttribBinding(vaoID, 1, 0);

	glVertexArrayVertexBuffer(vaoID, 0, vboID, 0, sizeof(float) * 4);
	Utils::checkError();
	glBindVertexArray(0);
}

void HUD::Bones::draw(std::string amount)
{
	//draw bone counter
	hudProgram.use();
	hudProgram.setTexture("hudImage", *texture);
	hudProgram.setMatrix4("projection", projection);
	glBindVertexArray(vaoID);
	glDrawArrays(GL_QUADS, 0, 5);

	gltBeginDraw();
	gltSetText(glText, ("x" + amount).c_str());
	gltColor(1, 1, 1, 1.0f);
	gltDrawText2D(glText, margin * 2 + size, margin * 2, 2.4);
	gltEndDraw();
	Utils::checkError();
}
