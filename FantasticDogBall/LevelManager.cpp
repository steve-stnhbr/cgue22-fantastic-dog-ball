#include "LevelManager.h"

#include "Utils.h"

Level* LevelManager::current = nullptr;
Player* LevelManager::playerTemplate;

LevelManager::LevelManager()
{
    levels.resize(Globals::NUM_LEVELS);

    playerTemplate = new Player();
    playerTemplate->init();
}

void LevelManager::load(unsigned short levelNr)
{
	switch (levelNr) {
	case 0:
        Level* level = new Level(playerTemplate);

        Light::Point p = {
            glm::vec3(0, 1, 0),
            2.0f, 1.0f, .5f,
            glm::vec3(1.5,1.5,1.5),
            glm::vec3(5,5,5),
            glm::vec3(2,2,2)
        };

        //level0->scene.lights.add(p);

        Light::Directional d = {
            glm::vec3(2, -8, 1),
            glm::vec3(.5),
            glm::vec3(1.3),
            glm::vec3(1.9),
            true
        };

        level->add(d);

        level->set(new Cubemap("../res/cubemaps/Storforsen4"));

        level->scene.lights.finalize();

        auto* ground_mat = new Material::TextureMaterial;
        ground_mat->color = { "../res/concrete.jpg" };
        ground_mat->normal = { "../res/concrete_norm.jpg" };
        ground_mat->diffuse = { .2 };
        ground_mat->specular = { .5 };
        ground_mat->shininess = .3;
        auto ground = new RenderObject(Render::Plane(20, 100), ground_mat, "Ground");
        ground->translate({ 0, -1, 0 });
        ground->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        level->add(ground);
        auto* cube = new RenderObject(Render::Cube(0, 0, 0, 1, 1, 1), ground_mat, "Cube");
        level->add(cube);
        level->finalize();
        levels[levelNr] = level;
        current = level;
        Inputs::setProcessor(current);
        break;
	}
}

void LevelManager::render()
{
    switch (state) {
    case PLAYING:
        current->render();
        break;
    }
}
