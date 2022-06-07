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
}

void Camera::setDirection(const glm::vec3 direction_)
{
	direction = direction_;
}

void Camera::setPitch(const float pitch_) {
	xRotation = pitch_;
}

void Camera::setYaw(const float angle_)
{
	yRotation = angle_;
}

void Camera::setRoll(const float roll_) {
	zRotation = roll_;
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

void Camera::bindWithModel(Shaders::Program& prog, glm::mat4 model)
{
	data.model = model; 
	buffer.update(sizeof(Data), &data);
	prog.setUniform("CameraData", buffer);
}

void Camera::update() {
	direction = glm::vec3(glm::sin(yRotation), -.5 + xRotation, glm::cos(yRotation));
	direction.y += (direction.y - xRotation) * .2;

	data.position = glm::vec4(glm::vec3(data.position) - glm::normalize(direction) * 5.0f, 1);

	const auto up = glm::mat3(glm::rotate(glm::mat4(1.0f), zRotation, direction)) * glm::vec3(.0f, 1.0f, .0f);

	const float ratio = static_cast<float>(Globals::WINDOW_WIDTH) / static_cast<float>(Globals::WINDOW_HEIGHT);
	data.projection = glm::perspective<float>(45, ratio, .1f, 100.0f);
	data.view = glm::lookAt<float>(glm::vec3(data.position), glm::vec3(data.position) + direction, up);
}
