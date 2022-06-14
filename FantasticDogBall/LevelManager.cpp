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
    //delete current;
    currentIndex = levelNr;
	switch (levelNr) {
    case 0: {
        state = State::PLAYING;
        Level* level = new Level(playerTemplate, 30);

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
        auto ground = new RenderObject(new Render::Plane(20, 50), ground_mat, "Ground");
        ground->translate({ 0, -1, 20 });
        ground->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        level->add(ground);

        auto elevated = new RenderObject(new Render::Cube(0, 0, 0, 20, 2, 40), ground_mat, "Elevated Ground");
        elevated->translate({ 0, 2, 65 });
        elevated->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        level->add(elevated);
        auto* treat = new Items::DogTreat(level->pWorld, glm::vec3(0, 0, 5));
        level->add(treat);
        auto* treat1 = new Items::DogTreat(level->pWorld, glm::vec3(0, 0, 10));
        level->add(treat1);
        auto* treat2 = new Items::DogTreat(level->pWorld, glm::vec3(0, 0, 15));
        level->add(treat2);
        auto* treat3 = new Items::DogTreat(level->pWorld, glm::vec3(0, 3, 35));
        level->add(treat3);
        auto* treat4 = new Items::DogTreat(level->pWorld, glm::vec3(0, 3, 40));
        level->add(treat4);
        auto* treat5 = new Items::DogTreat(level->pWorld, glm::vec3(0, 3, 45));
        level->add(treat5);

        auto* jumpPad = new Items::JumpPad(level->pWorld, glm::vec3(0, -1, 40), 5);
        level->add(jumpPad);

        auto* goal = new Items::Goal(glm::vec3(0, 4, 75));
        goal->rotate(glm::radians(90.f), {0, 1, 0});
        level->add(goal);

        level->finalize();
        current = level;
        Inputs::setProcessor(current);
    }
    break;
    case 2: {
        auto level = new Level(playerTemplate, 50);

        Light::Directional d = {
            glm::vec3(2, -8, -1),
            glm::vec3(.5),
            glm::vec3(1.3),
            glm::vec3(1.9),
            false
        };

        level->add(d);

        level->set(new Cubemap("../res/cubemaps/Yokohama2"));
        level->scene.lights.finalize();

        auto ground_material = new Material::TextureMaterial(
            new Texture::Texture("../res/grass/color.jpeg"),
            new Texture::Texture("../res/grass/normal.jpeg"),
            new Texture::Texture(.3),
            new Texture::Texture(.05),
            new Texture::Texture("../res/grass/shininess.jpeg"),
            1
        );
        auto physics = new Decoration::Physics(level->pWorld, nullptr, 0);

        const int offset = 25;
        const float rotation = glm::radians(10.f);

        auto platform1 = new RenderObject(new Render::Plane(18, 25), ground_material, "Platform1");
        platform1->translate({ 0, -2,  10});
        platform1->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        auto platform2 = new RenderObject(new Render::Plane(18, 30), ground_material, "Platform2");
        platform2
            ->rotate(rotation, { 0, 0, 1 })
            ->translate({ 10,-4, offset})
            ->rotate(glm::radians(30.f), { 0, 1,0 });
        platform2->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        auto platform3 = new RenderObject(new Render::Plane(18, 30), ground_material, "Platform3");
        platform3
            ->rotate(rotation * 2, { 0, 0, 1 })
            ->translate({ 10,-6, offset * 2})
            ->rotate(glm::radians(60.f), { 0, 1,0 });
        platform3->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        auto platform4 = new RenderObject(new Render::Plane(18, 18), ground_material, "Platform4");
        platform4
            ->rotate(rotation * 3, { 0, 0, 1 })
            ->translate({ 10,-8, offset * 3})
            ->rotate(glm::radians(90.f), { 0, 1,0 });
        platform4->add(new Decoration::Physics(level->pWorld, nullptr, 0));

        auto goal = new Items::Goal({ 0, -8, offset * 3 + 20 });

        level->add(platform1);
        level->add(platform2);
        level->add(platform3);
        level->add(platform4);
        level->add(goal);

        level->finalize();
        Inputs::setProcessor(level);
        state = State::PLAYING;
        current = level;
        break;
    }
    case 1: {
        auto level = new Level(playerTemplate, 50);

        Light::Directional d = {
            glm::vec3(2, -8, -1),
            glm::vec3(.5),
            glm::vec3(1.3),
            glm::vec3(1.9),
            true
        };

        level->add(d);

        level->set(new Cubemap("../res/cubemaps/Yokohama2"));
        level->scene.lights.finalize();

        auto ground_material = new Material::TextureMaterial(
            new Texture::Texture("../res/terazzo/color.jpg"),
            new Texture::Texture("../res/terazzo/normal.jpg"),
            new Texture::Texture(.3),
            new Texture::Texture(.5),
            new Texture::Texture("../res/terazzo/shininess.jpg"),
            1.2
        );
        auto physics = new Decoration::Physics(level->pWorld, nullptr, 0);

        const int offset = 30;
        const float rotation = glm::radians(10.f);

        auto platform1 = new RenderObject(new Render::Plane(18, 25), ground_material, "Platform1");
        platform1->translate({ 0, -2,  10 });
        platform1->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        auto platform2 = new RenderObject(new Render::Plane(8, 30), ground_material, "Platform2");
        platform2
            ->rotate(rotation, { 0, 0, 1 })
            ->translate({ 10,-4, offset });
        platform2->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        auto platform3 = new RenderObject(new Render::Plane(8, 30), ground_material, "Platform3");
        platform3
            ->rotate(-rotation, { 0, 0, 1 })
            ->translate({ 10,-6, offset * 2 });
        platform3->add(new Decoration::Physics(level->pWorld, nullptr, 0)); 
        auto platform4 = new RenderObject(new Render::Plane(18, 18), ground_material, "Platform4");
        platform4
            ->translate({ 10,-10, offset * 2 + 32});
        platform4->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        auto goal = new Items::Goal({ 0, -8, offset * 3 });

        level->add(platform1);
        level->add(platform2);
        level->add(platform3);
        level->add(platform4);
        level->add(goal);

        level->finalize();
        Inputs::setProcessor(level);
        state = State::PLAYING;
        current = level;
        break;
    }
    case 3: {
        auto level = new Level(playerTemplate, 50);

        Light::Directional d = {
            glm::vec3(2, -8, -1),
            glm::vec3(.5),
            glm::vec3(1.3),
            glm::vec3(1.9),
            true
        };

        level->add(d);

        level->scene.lights.finalize();
        level->set(new Cubemap("../res/cubemaps/Yokohama2"));

        auto slide = new RenderObject(
            Render::Mesh::fromFile("../res/Slide.obj")[0],
            new Material::TextureMaterial(
                new Texture::Texture("../res/terracotta/color.jpg"),
                new Texture::Texture("../res/terracotta/normal.jpg"),
                new Texture::Texture(.5),
                new Texture::Texture(.4),
                new Texture::Texture("../res/terracotta/shininess.jpg"),
                1.4f
            ),
            "Slide"
        );

        slide->add(new Decoration::Physics(level->pWorld, nullptr, 0));

        auto goal = new Items::Goal({ 0, 33, -7.8 });

        level->add(goal);
        level->add(slide);

        level->finalize();
        Inputs::setProcessor(level);
        state = State::PLAYING;
        current = level;
        break;
    }
    case 4: {
        auto level = new Level(playerTemplate, 50);

        Light::Directional d = {
            glm::vec3(2, -8, -1),
            glm::vec3(.5),
            glm::vec3(1.3),
            glm::vec3(1.9),
            true
        };

        level->add(d);

        level->scene.lights.finalize();
        level->set(new Cubemap("../res/cubemaps/Yokohama2"));

        auto ground = new RenderObject(
            Render::Mesh::fromFile("../res/HoleLevel.obj")[0],
            new Material::TextureMaterial(
                new Texture::Texture("../res/emerald_tiles/color.jpg"),
                new Texture::Texture("../res/emerald_tiles/normal.jpg"),
                new Texture::Texture(.5f),
                new Texture::Texture(.7f),
                new Texture::Texture("../res/emerald_tiles/shininess.jpg"),
                2.3
            ),
            "HoleyGround"
        );

        ground->add(new Decoration::Physics(level->pWorld, nullptr, 0));

        level->add(ground);

        level->finalize();
        Inputs::setProcessor(level);
        state = State::PLAYING;
        current = level;
        break;
    }
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
    //delete current;
    delete startMenu;
    delete timeOverMenu;
    delete gameOverMenu;
    delete finishedMenu;
}
