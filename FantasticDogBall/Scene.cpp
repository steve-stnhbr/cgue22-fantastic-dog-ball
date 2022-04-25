#include "Scene.h"


void Scene::addObject(RenderObject object)
{
    object.index = objects.size();
	objects.push_back(object);

    // todo modify number óf light sources
}

void Scene::addObject(RenderObject* object)
{
    addObject(*object);
}

void Scene::render()
{
    if (lights.empty<Light::Point>())
        lights.add(Light::Point());
    if (lights.empty<Light::Point>())
        lights.add(Light::Directional());
    if (lights.empty<Light::Point>())
        lights.add(Light::Spot());

    renderer.render(objects, lights);
}
