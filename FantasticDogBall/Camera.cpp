#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Utils.h"
#include "LevelManager.h"

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

void Camera::setPlayerPosition(const glm::vec3 p)
{
	playerPos = p;
}

void Camera::setDirection(const glm::vec3 direction_)
{
	direction = direction_;
}

void Camera::setLeaning(const float leaning_)
{
	leaning = leaning_;
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

void Camera::bindCubemap(Shaders::Program& prog)
{
	Data dataCopy;
	dataCopy.projection = glm::perspective<float>(45, 1, .1f, 100.0f);
	dataCopy.view = glm::mat4(glm::mat3(glm::lookAt<float>(glm::vec3(data.position), glm::vec3(data.position) + direction, { 0, 1, 0 })));
	buffer.update(sizeof(Data), &dataCopy);
	prog.setUniform("CameraData", buffer);
}

void Camera::update() {
	// interpolate the leaning for smooth transition
	a_leaning += (leaning - a_leaning) * .14;
	direction = glm::vec3(glm::sin(yRotation), -.47, glm::cos(yRotation));
	
	// move camera away from the player 
	data.position = glm::vec4(glm::vec3(playerPos) - glm::normalize(direction) * 5.0f, 1);

	// rotate camera depending on gravity 
	// todo x rotation on w and s
	const auto gravityAxis = glm::vec3(LevelManager::current->pWorld->getGravity().x(), LevelManager::current->pWorld->getGravity().z(), 1);
	const auto rotationAxis = glm::cross(direction, gravityAxis);
	const auto up = glm::vec3(glm::rotate(glm::mat4(1.0f), a_leaning, rotationAxis) * glm::vec4(0,1.0f,0,0));

	const float ratio = static_cast<float>(Globals::WINDOW_WIDTH) / static_cast<float>(Globals::WINDOW_HEIGHT);
	data.projection = glm::perspective<float>(45, ratio, .1f, 100.0f);
	data.view = glm::lookAt<float>(glm::vec3(data.position), glm::vec3(data.position) + direction, up);
}
