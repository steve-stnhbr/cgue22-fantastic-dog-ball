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
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		/**
		 * The vector describing the cameras position;
		 */
		glm::vec4 position;
		glm::vec3 direction;
	};

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

	void setProjection(const glm::mat4);

	void setView(const glm::mat4);

	void setRotation(const glm::vec3);

	void bind(Shaders::Program&);

	/**
	 * Function that binds the camera to the currently active program
	 */
	void bindWithModel(Shaders::Program&, glm::mat4);
	/**
	 * Struct containing current state of the camera
	 */
	Data data;
};

