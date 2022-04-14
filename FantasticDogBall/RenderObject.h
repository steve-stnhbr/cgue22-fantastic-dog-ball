#pragma once
#include "Render.h"

/**
 * 
 */
class RenderObject
{
public:
	/**
	 * The Mesh containing the vertecies and indices used to draw
	 */
	Render::Mesh		mesh;
	/*
	 * A pointer to the material the RenderObject is using
	 */
	Render::Material*	material;
	/*
	 * This string is just for debugging purposes
	 */
	std::string			name;
	/**
	 * The index of the render object in the scene, set when adding object to the scene
	 */
	unsigned long long	index;
	/**
	 * unsigned 
	 */
	GLuint vaoID, vboID, eboID;
	/*
	 * matrix holding information about transforms
	 */
	glm::mat4 transform;

	RenderObject(Render::Mesh, Render::Material*, const std::string&);

	void buildVAO() const;
	void translate(float, float, float);
	void rotate(float, float, float);
	void scale(float, float, float);
	
};

class Cube : public RenderObject
{
	Cube(float centerX, float centerY, float centerZ, float width, float height, float depth);
};