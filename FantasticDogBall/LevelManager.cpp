#include "LevelManager.h"

#include "Utils.h"
#include "Items.h"

Level* LevelManager::current = nullptr;
Player* LevelManager::playerTemplate;
State LevelManager::state = State::START_MENU;
std::vector<Level*> LevelManager::levels;
unsigned LevelManager::currentIndex = -1;

LevelManager::LevelManager()
{
    levels.resize(Globals::NUM_LEVELS);

    playerTemplate = new Player();
    playerTemplate->init();

    gameOverMenu = new Menu::GameOverMenu();
    startMenu = new Menu::StartMenu();
    timeOverMenu = new Menu::TimeOverMenu();
}

void LevelManager::load(unsigned short levelNr)
{
    delete current;
    state = State::PLAYING;
    currentIndex = levelNr;
	switch (levelNr) {
	case 0:
        Level* level = new Level(playerTemplate, 100);

        Light::Point p = {
            glm::vec3(0, 1, 0),
            2.0f, 1.0f, .5f,
            glm::vec3(1.5,1.5,1.5),
            glm::vec3(5,5,5),
            glm::vec3(2,2,2)
        };

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
        auto* treat = new Items::DogTreat(level->pWorld, glm::vec3(0,0,5));
        level->add(treat);
        auto* treat1 = new Items::DogTreat(level->pWorld, glm::vec3(0, 0, 6));
        level->add(treat1);
        auto* treat2 = new Items::DogTreat(level->pWorld, glm::vec3(0, 0, 7));
        level->add(treat2);
        
        auto* goal = new Items::Goal(glm::vec3(0, 0, 12));
        level->add(goal);

        level->finalize();
        current = level;
        Inputs::setProcessor(current);
        break;
	}
}

void LevelManager::render()
{
    switch (state) {
    case State::LEVEL_FINISHED:
    case State::START_MENU:
        startMenu->draw();
        Inputs::setProcessor(startMenu);
        break;
    case State::PLAYING:
        state = current->render();
        break;
    case State::GAME_OVER:
        gameOverMenu->draw();
        Inputs::setProcessor(gameOverMenu);
        break;
    case State::TIME_OVER:
        timeOverMenu->draw();
        Inputs::setProcessor(timeOverMenu);
        break;
    }
}
