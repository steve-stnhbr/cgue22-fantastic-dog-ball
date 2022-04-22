#include "Camera.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(): data({ glm::mat4(1), glm::mat4(1) })
{
	buffer.create(sizeof(Data));
}

void Camera::setData(Data data_)
{
	data = data_;
	// buffer.update(sizeof(data), &data);
}

void Camera::setPosition(const glm::vec3 position_)
{
	// data.viewPos = position_;
}

void Camera::setProjection(const glm::mat4 mat)
{
	data.projection = mat;
}

void Camera::setView(const glm::mat4 mat)
{
	data.view = mat;
}

void Camera::bind(Shaders::Program program)
{
	bindWithModel(program, glm::mat4(1));
}


void Camera::bindWithModel(Shaders::Program prog, glm::mat4 model)
{
	data.model = model; 
	buffer.update(sizeof(Data), &data);
	prog.setUniform("CameraData", buffer);
}


