#include "Scene.h"


void Scene::addObject(RenderObject object)
{
    object.index = objects.size();
	objects.push_back(object);
}

/*
void Scene::removeObject(const RenderObject& object)
{
    for (auto it = objects.begin(); it != objects.end();) {
        if (it->index == object.index) {
            it = objects.erase(it);
        } 
        else {
            ++it;
        }
    }
}
*/



void Scene::render()
{
    renderer.render(objects);
}

