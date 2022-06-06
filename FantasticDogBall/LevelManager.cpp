#include "LevelManager.h"

#include "Utils.h"

LevelManager::LevelManager()
{
    levels.resize(Globals::NUM_LEVELS);
}

void LevelManager::load(unsigned short levelNr)
{
	switch (levelNr) {
	case 0:
        Level* level = new Level();

        Light::Point p = {
            glm::vec3(0, 1, 0),
            2.0f, 1.0f, .5f,
            glm::vec3(1.5,1.5,1.5),
            glm::vec3(5,5,5),
            glm::vec3(2,2,2)
        };

        //level0->scene.lights.add(p);

        Light::Directional d = {
            glm::vec3(2, -4, 1),
            glm::vec3(.5),
            glm::vec3(.8),
            glm::vec3(.9),
            false
        };

        level->add(d);

        level->scene.lights.finalize();

        level->init();

        auto* ground_mat = new Material::TextureMaterial;
        ground_mat->color = { "../res/concrete.jpg" };
        ground_mat->normal = { "../res/concrete_norm.jpg" };
        ground_mat->diffuse = { .2 };
        ground_mat->specular = { .5 };
        ground_mat->shininess = .3;
        auto ground = new RenderObject(Render::Plane(100, 100), ground_mat, "Ground");
        ground->translate({ 0, -4, 0 });
        ground->rotate(.27, { 1, 0, 1 });
        ground->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        level->add(ground);
        levels[levelNr] = level;
        current = level;
        break;
	}
}

void LevelManager::render()
{
    current->render();
}
