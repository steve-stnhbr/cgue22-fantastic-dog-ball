#pragma once

#include "RenderObject.h"
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>

class Items
{


	class DogTreat : public RenderObject {

	public:
		DogTreat(btDynamicsWorld*);
		DogTreat(btDynamicsWorld*, glm::vec3 position);

		void onCollide(RenderObject* other);
	};
};

