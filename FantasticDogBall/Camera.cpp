#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Utils.h"

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
	data.position = glm::vec4(position_.x, position_.y, position_.z, 0);
	update();
}

void Camera::setDirection(const glm::vec3 dir)
{
	direction = dir;
	update();
}

void Camera::setProjection(const glm::mat4 mat)
{
	data.projection = mat;
}

void Camera::setView(const glm::mat4 mat)
{
	data.view = mat;
}

void Camera::bind(Shaders::Program& program)
{
	bindWithModel(program, glm::mat4(1));
}

void Camera::update()
{
	const float ratio = static_cast<float>(Globals::WINDOW_WIDTH) / static_cast<float>(Globals::WINDOW_HEIGHT);

	data.projection = glm::perspective<float>(45, ratio, .1f, 100.0f);
	data.view = glm::lookAt<float>(glm::vec3(data.position), glm::vec3(data.position) + direction, { .0f, 1.0f, .0f });
	data.position = glm::vec4(data.position.x, data.position.y, data.position.z, 1);
}

void Camera::bindWithModel(Shaders::Program& prog, glm::mat4 model)
{
	data.model = model; 
	buffer.update(sizeof(Data), &data);
	prog.setUniform("CameraData", buffer);
}
