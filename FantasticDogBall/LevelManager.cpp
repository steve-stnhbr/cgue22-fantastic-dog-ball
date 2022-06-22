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

        level->set(new Cubemap(Globals::RESOURCES + "/cubemaps/Yokohama2"));

        level->scene.lights.finalize();

        auto* ground_mat = new Material::TextureMaterial;
        ground_mat->color = new Texture::Texture{ Globals::RESOURCES + "/concrete.jpg" };
        ground_mat->normal = new Texture::Texture{ Globals::RESOURCES + "/concrete_norm.jpg" };
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
        auto* treat3 = new Items::DogTreat(level->pWorld, glm::vec3(0, 4, 50));
        level->add(treat3);
        auto* treat4 = new Items::DogTreat(level->pWorld, glm::vec3(0, 4, 55));
        level->add(treat4);
        auto* treat5 = new Items::DogTreat(level->pWorld, glm::vec3(0, 4, 60));
        level->add(treat5);

        auto* jumpPad = new Items::JumpPad(level->pWorld, glm::vec3(0, -1, 40), 3);
        level->add(jumpPad);

        auto* goal = new Items::Goal(glm::vec3(0, 3, 75));
        goal->rotate(glm::radians(90.f), {0, 1, 0});
        level->add(goal);

        level->finalize();
        current = level;
        Inputs::setProcessor(current);
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

        level->set(new Cubemap(Globals::RESOURCES + "/cubemaps/NissiBeach"));
        level->scene.lights.finalize();

        auto ground_material = new Material::TextureMaterial(
            new Texture::Texture(Globals::RESOURCES + "/terazzo/color.jpg"),
            new Texture::Texture(Globals::RESOURCES + "/terazzo/normal.jpg"),
            new Texture::Texture(.3),
            new Texture::Texture(.5),
            new Texture::Texture(Globals::RESOURCES + "/terazzo/shininess.jpg"),
            1.2
        );
        auto physics = new Decoration::Physics(level->pWorld, nullptr, 0);

        const int offset = 30;
        const float rotation = glm::radians(20.f);

        auto platform1 = new RenderObject(new Render::Plane(18, 25), ground_material, "Platform1");
        platform1->translate({ 0, -2,  10 });
        platform1->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        auto platform2 = new RenderObject(new Render::Plane(8, 30), ground_material, "Platform2");
        platform2
            ->translate({ 10,-4, offset })
            ->rotate(rotation, { 0, 0, 1 });
        platform2->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        auto platform3 = new RenderObject(new Render::Plane(8, 30), ground_material, "Platform3");
        platform3
            ->translate({ 8,-6, offset * 2 })
            ->rotate(-rotation, { 0, 0, 1 });
        platform3->add(new Decoration::Physics(level->pWorld, nullptr, 0)); 
        auto platform4 = new RenderObject(new Render::Plane(18, 18), ground_material, "Platform4");
        platform4
            ->translate({ 10,-10, offset * 2 + 20});
        platform4->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        auto goal = new Items::Goal({ 5, -10, offset * 2 + 20 });

        level->add(platform1);
        level->add(platform2);
        level->add(new Items::DogTreat(level->pWorld, { 10, -3.5, 35 }));
        level->add(new Items::DogTreat(level->pWorld, { 10, -3.5, 40 }));
        level->add(new Items::DogTreat(level->pWorld, { 10, -3.5, 45 }));
        level->add(platform3);
        level->add(new Items::DogTreat(level->pWorld, { 8, -5.5, 65 }));
        level->add(new Items::DogTreat(level->pWorld, { 8, -5.5, 70 }));
        level->add(new Items::DogTreat(level->pWorld, { 8, -5.5, 75 }));
        level->add(platform4);
        level->add(goal);

        level->finalize();
        Inputs::setProcessor(level);
        state = State::PLAYING;
        current = level;
        break;
    }
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

        level->set(new Cubemap(Globals::RESOURCES + "/cubemaps/Yokohama2"));
        level->scene.lights.finalize();

        auto ground_material = new Material::TextureMaterial(
            new Texture::Texture(Globals::RESOURCES + "/sci-fi/color.jpg"),
            new Texture::Texture(Globals::RESOURCES + "/sci-fi/normal.jpg"),
            new Texture::Texture(.3),
            new Texture::Texture(.05),
            new Texture::Texture(Globals::RESOURCES + "/sci-fi/shininess.jpg"),
            1
        );
        auto physics = new Decoration::Physics(level->pWorld, nullptr, 0);

        const int offset = 25;
        const float rotation = glm::radians(10.f);

        auto platform1 = new RenderObject(new Render::Plane(18, 25), ground_material, "Platform1");
        platform1->translate({ 0, 0,  10 });
        platform1->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        auto platform2 = new RenderObject(new Render::Plane(18, 25), ground_material, "Platform1");
        platform2->translate({ 0, -5,  20 });
        platform2->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        auto platform3 = new RenderObject(new Render::Plane(18, 25), ground_material, "Platform1");
        platform3->translate({ 0, -10,  10 });
        platform3->add(new Decoration::Physics(level->pWorld, nullptr, 0));
        auto platform4 = new RenderObject(new Render::Plane(18, 25), ground_material, "Platform1");
        platform4->translate({ 0, -15,  20 });
        platform4->add(new Decoration::Physics(level->pWorld, nullptr, 0));

        auto goal = new Items::Goal({ 0, -15, 20 });

        level->add(platform1);
        level->add(new Items::DogTreat(level->pWorld, { -5, 1, 5 }));
        level->add(new Items::DogTreat(level->pWorld, { 5, 1, 5 }));
        level->add(new Items::DogTreat(level->pWorld, { 5, 1, 15 }));
        level->add(new Items::DogTreat(level->pWorld, { -5, 1, 15 }));
        level->add(platform2);
        level->add(new Items::DogTreat(level->pWorld, { -5, -4, 15 }));
        level->add(new Items::DogTreat(level->pWorld, { 5, -4, 15 }));
        level->add(new Items::DogTreat(level->pWorld, { 5, -4, 25 }));
        level->add(new Items::DogTreat(level->pWorld, { -5, -4, 25 }));
        level->add(platform3);
        level->add(new Items::DogTreat(level->pWorld, { -5, -9, 5 }));
        level->add(new Items::DogTreat(level->pWorld, { 5, -9, 5 }));
        level->add(new Items::DogTreat(level->pWorld, { 5, -9, 15 }));
        level->add(new Items::DogTreat(level->pWorld, { -5, -9, 15 }));
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
        level->set(new Cubemap(Globals::RESOURCES + "/cubemaps/Yokohama2"));

        auto ground = new RenderObject(
            Render::Mesh::fromFile(Globals::RESOURCES + "/HoleLevel.robj")[0],
            new Material::TextureMaterial(
                new Texture::Texture(Globals::RESOURCES + "/emerald_tiles/color.jpg"),
                new Texture::Texture(Globals::RESOURCES + "/emerald_tiles/normal.jpg"),
                new Texture::Texture(.5f),
                new Texture::Texture(.7f),
                new Texture::Texture(Globals::RESOURCES + "/emerald_tiles/shininess.jpg"),
                2.3
            ),
            "HoleyGround"
        );
        ground->add(new Decoration::Physics(level->pWorld, nullptr, 0));

        auto goal = new Items::Goal({ 5, 0, 36 });


        std::vector<glm::vec3> bones = {
            {-3, 0, 22},
            {-4.3, 0, 17},
            {9, 0, 20},
            {6.8, 0, 13},
            {12, 0, .6},
            {-6.7, 0, 1.87},
            {-7.18, 0, 9.93},
            {-11.8, 0, 17.3},
            {-7, 0, 21},
            {-1.14, 0, 13},
            {2.96, 0, 22},
            {-11.69, 0, 6.5},
            {-2.96, 0, -9.11},
        };

        for (auto vec : bones) {
            auto* bone = new Items::DogTreat(level->pWorld, vec * glm::vec3(-1.4, 1.4, 1.4) + glm::vec3{0, 1, 0});
            level->add(bone);
        }

        level->add(ground);
        level->add(goal);

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
        level->set(new Cubemap(Globals::RESOURCES + "/cubemaps/Yokohama2"));

        auto slide = new RenderObject(
            Render::Mesh::fromFile(Globals::RESOURCES + "/SlideSubdiv_.robj")[0],
            new Material::TextureMaterial(
                new Texture::Texture(Globals::RESOURCES + "/terracotta/color.jpg"),
                new Texture::Texture(Globals::RESOURCES + "/terracotta/normal.jpg"),
                new Texture::Texture(.5),
                new Texture::Texture(.4),
                new Texture::Texture(Globals::RESOURCES + "/terracotta/shininess.jpg"),
                1.4f
            ),
            "Slide"
        );

        slide->add(new Decoration::Physics(level->pWorld, nullptr, 0));

        auto goal = new Items::Goal({ 50, -12, 50 });

        level->add(new Items::DogTreat(level->pWorld, { 0, -2       +1, 3.16 }));
        level->add(new Items::DogTreat(level->pWorld, { 0, -2.716 + 1, 6 }));
        level->add(new Items::DogTreat(level->pWorld, { 0, -3.9 + 1, 9 }));
        level->add(new Items::DogTreat(level->pWorld, { 0, -7.15 + 1, 18 }));
        level->add(new Items::DogTreat(level->pWorld, { 0, -7.62 + 1, 21 }));
        level->add(new Items::DogTreat(level->pWorld, { 0, -5.5 + 1, 37 }));
        level->add(new Items::DogTreat(level->pWorld, { 0, -5.54 + 1 , 40 }));
        level->add(new Items::DogTreat(level->pWorld, { 0, -5.54 + 1, 43 }));
        level->add(new Items::DogTreat(level->pWorld, { 14.15, -7.96 + 1, 50 }));
        level->add(new Items::DogTreat(level->pWorld, { 17.35, -9.18 + 1, 50 }));
        level->add(new Items::DogTreat(level->pWorld, { 20.2, -10.11 + 1, 50 }));

        level->add(goal);
        level->add(slide);

        level->finalize();
        Inputs::setProcessor(level);
        state = State::PLAYING;
        current = level;
        break;
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
