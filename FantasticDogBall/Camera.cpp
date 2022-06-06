#include "Camera.h"
#include "Camera.h"
#include "Utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(): data({ glm::mat4(1), glm::mat4(1) })
{
	const glm::vec3 cameraPos = glm::vec3(0, 1, -6);

	const float ratio = static_cast<float>(Globals::WINDOW_WIDTH) / static_cast<float>(Globals::WINDOW_HEIGHT);

	const auto proj = glm::perspective<float>(45, ratio, .1f, 100.0f);
	const auto view = glm::lookAt<float>(cameraPos, { .0f, .0f, .0f }, { .0f, 1.0f, .0f });
	data = Camera::Data{ glm::mat4(1), view, proj, glm::vec4(cameraPos.x, cameraPos.y, cameraPos.z, 1) };
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
