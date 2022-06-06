#pragma once

#include "RenderObject.h"
#include "Level.h"

class Player : public RenderObject
{
private:
	Decoration::Animation stand, walk, trot, canter;
public:
	RenderObject dog, ball;
	Player(Level& level);
	Player(Level& level, glm::vec3 position);
	void update(unsigned long frame, float dTime);
	void draw(Shaders::Program prog) override;
};

