#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shaders.h"
#include "UniformBuffer.h"

class Camera
{
public:
	/**
	 * Struct for representing the camera data in shaders
	 */
	struct Data {
		glm::mat4 view;
		glm::mat4 projection;
	};

	struct ShaderData
	{
		glm::mat4 mvp;
	};

	/**
	 * Default constructor
	 */
	Camera();

	/**
	 * The buffer for camera data
	 */
	UniformBuffer buffer;

	/**
	 * Function to update the cameras data
	 */
	void setData(Data);

	void setProjection(const glm::mat4);

	void setView(const glm::mat4);

	/**
	 * Function that binds the camera to the currently active program
	 */
	void bindCamera(Shaders::Program, glm::mat4);

private:
	/**
	 * Struct containing current state of the camera
	 */
	Data data;

};

