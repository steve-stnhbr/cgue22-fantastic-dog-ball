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
	Light::Lights<Light::Point> pLights;
	Light::Lights<Light::Directional> dLights;
	Light::Lights<Light::Spot> sLights;
	Renderer renderer;

	Scene() = default;

	void addObject(RenderObject object);
	void removeObject(const RenderObject& object);

	void addLight(Light::Point light);
	void addLight(Light::Directional light);
	void addLight(Light::Spot light);

	void render();

};

