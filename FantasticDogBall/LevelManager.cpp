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
    finishedMenu = new Menu::FinishedMenu();
}

void LevelManager::load(unsigned short levelNr)
{
    delete current;
    currentIndex = levelNr;
	switch (levelNr) {
    case 0: {
        state = State::PLAYING;
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

        level->set(new Cubemap("../res/cubemaps/Yokohama2"));

        level->scene.lights.finalize();

        auto* ground_mat = new Material::TextureMaterial;
        ground_mat->color = new Texture::Texture{ "../res/concrete.jpg" };
        ground_mat->normal = new Texture::Texture{ "../res/concrete_norm.jpg" };
        ground_mat->diffuse = new Texture::Texture{ .2 };
        ground_mat->specular = new Texture::Texture{ .5 };
        ground_mat->shininess = .3;
        auto ground = new RenderObject(new Render::Plane(20, 100), ground_mat, "Ground");
        ground->translate({ 0, -1, 0 });
        ground->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        level->add(ground);
        auto* treat = new Items::DogTreat(level->pWorld, glm::vec3(0, 0, 5));
        level->add(treat);
        auto* treat1 = new Items::DogTreat(level->pWorld, glm::vec3(0, 0, 6));
        level->add(treat1);
        auto* treat2 = new Items::DogTreat(level->pWorld, glm::vec3(0, 0, 7));
        level->add(treat2);

        auto* jumpPad = new Items::JumpPad(level->pWorld, glm::vec3(0, 0, 10), 5);
        level->add(jumpPad);

        auto* goal = new Items::Goal(glm::vec3(0, 0, 24));
        level->add(goal);

        level->finalize();
        current = level;
        Inputs::setProcessor(current);
    }
    break;
    default:
        Loggger::error("Level %i not defined", levelNr);
	}
}

void LevelManager::render()
{
    switch (state) {
    case State::LEVEL_FINISHED:
        finishedMenu->draw(current->initialTime - current->time, current->bones);
        Inputs::setProcessor(finishedMenu);
        break;
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

LevelManager::~LevelManager()
{
    delete current->player;
    delete current;
    delete startMenu;
    delete timeOverMenu;
    delete gameOverMenu;
    delete finishedMenu;
}
