#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "UniformBuffer.h"

class Camera
{
public:
	struct Data {
		glm::mat4 view;
		glm::mat4 projection;
	};

	Camera();
	~Camera();

	UniformBuffer buffer;

	void setData(Data);

private:
	Data data;

};

