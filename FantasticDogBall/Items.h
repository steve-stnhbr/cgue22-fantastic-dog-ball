#pragma once

#include "RenderObject.h"
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>

namespace Items
{


	class DogTreat : public RenderObject {

	public:
		bool exists;

		DogTreat(btDynamicsWorld*);
		DogTreat(btDynamicsWorld*, glm::vec3 position);

		void onCollide(RenderObject* other);
	};

	class Goal : public RenderObject {
	public:
		Goal();
		Goal(glm::vec3 position);
	};
};

