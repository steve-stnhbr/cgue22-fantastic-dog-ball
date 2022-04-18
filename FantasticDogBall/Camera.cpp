#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(): data({ glm::mat4(1), glm::mat4(1) })
{
	buffer.create(sizeof(glm::mat4));
}

void Camera::setData(Data data_)
{
	data = data_;
	// buffer.update(sizeof(data), &data);
}

void Camera::setProjection(const glm::mat4 mat)
{
	data.projection = mat;
}

void Camera::setView(const glm::mat4 mat)
{
	data.view = mat;
}


void Camera::bindCamera(Shaders::Program prog, glm::mat4 model)
{
	ShaderData d = { data.projection * data.view * model };
	buffer.update(sizeof(glm::mat4), &d);
	prog.setUniform("CameraData", buffer);
}


