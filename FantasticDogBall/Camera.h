#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	struct Data {
		glm::mat4 view;
		glm::mat4 projection;
	};
};

