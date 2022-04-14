#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera(): data({ glm::mat4(1), glm::mat4(1) })
{
}


void Camera::setData(Data data_)
{
	data = data_;
	buffer.update(sizeof(data), &data);
}

