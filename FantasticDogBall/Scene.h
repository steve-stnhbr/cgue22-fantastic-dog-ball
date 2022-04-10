#pragma once
#include "Renderer.h"
#include "RenderObject.h"
/*
 * The scene class is responsible for handling all RenderObjects
 *
 */
class Scene
{
public:
	std::vector<RenderObject> objects;
	Renderer renderer;

	void addObject(RenderObject object);

	void removeObject(const RenderObject& object);

	void render();

};

