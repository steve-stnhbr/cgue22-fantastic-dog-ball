#include "LevelManager.h"

#include "Utils.h"

Level* LevelManager::current = nullptr;
HUD* LevelManager::hud;

LevelManager::LevelManager()
{
    levels.resize(Globals::NUM_LEVELS);
    hud = new HUD();
    hud->init();
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
            glm::vec3(2, -8, 1),
            glm::vec3(.5),
            glm::vec3(1.3),
            glm::vec3(1.9),
            true
        };

        level->add(d);

        //level->set(new Cubemap("../res/cubemaps/Storforsen4"));

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
    current->render();
    start2D();
    hud->draw("32", "55");
    end2D();
}

void LevelManager::start2D()
{
    glUseProgram(0);
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glViewport(0, 0, Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT);
}

void LevelManager::end2D()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
}
