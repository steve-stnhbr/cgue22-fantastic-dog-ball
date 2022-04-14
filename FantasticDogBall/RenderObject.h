#pragma once
#include "Render.h"


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

	RenderObject(Render::Mesh, Render::Material*, const std::string&);

	void buildVAO() const;
};
