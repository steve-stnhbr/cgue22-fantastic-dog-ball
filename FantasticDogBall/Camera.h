#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

private:
	/**
	 * Struct containing current state of the camera
	 */
	Data data;

};

