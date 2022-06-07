#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shaders.h"
#include "UniformBuffer.h"
#include <glm/fwd.hpp>

class Camera
{
public:
	/**
	 * Struct for representing the camera data in shaders
	 */
	struct Data {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		/**
		 * The vector describing the cameras position;
		 */
		glm::vec4 position;
	} data;

	glm::vec3 direction;
	/*
	 * These variables hold the desired values for rotations
	*/
	float xRotation, yRotation, zRotation;
	/*
	 * These variables hold the actual values for rotations
	*/
	float a_xRotation, a_yRotation, a_zRotation;

	/**
	 * Default constructor
	 */
	Camera();

	/**
	 * The buffer for camera data
	 */
	UncheckedUniformBuffer buffer; 

	/**
	 * Function to update the cameras data
	 */
	void setData(Data);

	void setPosition(const glm::vec3);

	void setDirection(const glm::vec3);

	void setPitch(const float);

	void setYaw(const float);
	void setRoll(const float);

	void setProjection(const glm::mat4);

	void setView(const glm::mat4);

	void bind(Shaders::Program&);
	void update();

	/**
	 * Function that binds the camera to the currently active program
	 */
	void bindWithModel(Shaders::Program&, glm::mat4);
};

