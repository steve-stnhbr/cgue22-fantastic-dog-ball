#pragma once
#include "LightSource.h"
#include "Renderer.h"
#include "RenderObject.h"

#include "bullet/btBulletDynamicsCommon.h"

/*
 * The scene class is responsible for handling all RenderObjects
 *
 */
class Scene
{
public:
	std::vector<RenderObject> objects;
	Light::Lights lights;
	Renderer renderer;

	Scene() = default;

	void addObject(RenderObject object);

	void render();

};

