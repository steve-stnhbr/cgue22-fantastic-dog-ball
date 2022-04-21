#include "Scene.h"


void Scene::addObject(RenderObject object)
{
    object.index = objects.size();
	objects.push_back(object);
}

void Scene::render()
{
    if (pLights.empty())
        pLights.push_back(Light::Point());
    if (dLights.empty())
        dLights.push_back(Light::Directional());
    if (sLights.empty())
        sLights.push_back(Light::Spot());

    renderer.render(objects, pLights, dLights, sLights);
}

void Scene::addLight(Light::Point light)
{
    pLights.push_back(light);
}

void Scene::addLight(Light::Directional light)
{
    dLights.push_back(light);
}

void Scene::addLight(Light::Spot light)
{
    sLights.push_back(light);
}

