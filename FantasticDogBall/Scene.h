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
	std::vector<Light::Light<std::_Any_tag>> lights;
	Renderer renderer;

	void addObject(RenderObject object);

	void removeObject(const RenderObject& object);

	void render();

};

