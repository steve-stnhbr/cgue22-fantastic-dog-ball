#include "Scene.h"

void Scene::addObject(RenderObject object)
{
    addObject(&object);
}

void Scene::addObject(RenderObject* object)
{
    object->index = objects.size();
    objects.push_back(object);
}

void Scene::removeObject(RenderObject* object)
{
    objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
}

void Scene::render(float dTime)
{
    renderer.render(objects, lights, cubemap, dTime);
}