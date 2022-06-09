#pragma once

#include "RenderObject.h"
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>

class Player : public RenderObject
{
private:
	Decoration::Animation *stand, *walk, *trot, *canter;
public:
	float directionAngle;
	RenderObject *dog, *ball;
	Player();
	Player(glm::vec3 position);

	void init();
	void init(btDynamicsWorld* world);
	void update(unsigned long frame, float dTime);
	void draw(Shaders::Program prog) override;
};

