#include "Menu.h"

#include "LevelManager.h"

Menu::StartMenu::StartMenu()
{
	gltInit();
	heading = gltCreateText();
	gltSetText(heading, "FantasticDogBall");
	text = gltCreateText();
	gltSetText(text, "Press keys 1 to 9 to select a level");
}

void Menu::StartMenu::draw()
{
	gltBeginDraw();
	gltDrawText2DAligned(heading, Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT / 2, 3, GLT_CENTER, GLT_CENTER);
	gltDrawText2DAligned(heading, Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT / 2, 3, GLT_CENTER, GLT_CENTER);
	gltEndDraw();
}

void Menu::StartMenu::on(int key)
{
	if (key >= 49 && key <= 57) { // numbers from 1 to 9
		LevelManager::load(key - 49);
	}
}

Menu::TimeOverMenu::TimeOverMenu()
{
	gltInit();
	heading = gltCreateText();
	gltSetText(heading, "Time is over");
	text = gltCreateText();
	gltSetText(text, "To start over press r, to return to the start menu press e");
}

void Menu::TimeOverMenu::draw()
{
	gltBeginDraw();
	gltDrawText2DAligned(heading, Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT / 2, 3, GLT_CENTER, GLT_CENTER);
	gltDrawText2DAligned(heading, Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT / 2, 3, GLT_CENTER, GLT_CENTER);
	gltEndDraw();
}

void Menu::TimeOverMenu::on(int key)
{
	switch (key) {
	case GLFW_KEY_E:
		LevelManager::state = State::START_MENU;
		break;
	case GLFW_KEY_R:
		LevelManager::load(LevelManager::currentIndex);
		break;
	}
}

Menu::GameOverMenu::GameOverMenu()
{
	gltInit();
	heading = gltCreateText();
	gltSetText(heading, "Game Over");
	text = gltCreateText();
	gltSetText(text, "To start over press r, to return to the start menu press e");
}

void Menu::GameOverMenu::draw()
{
	gltColor(0.f, 0.f, 0.f, 0.f);
	gltBeginDraw();
	gltDrawText2DAligned(heading, Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT / 2, 3, GLT_CENTER, GLT_CENTER);
	gltDrawText2DAligned(heading, Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT / 2, 3, GLT_CENTER, GLT_CENTER);
	gltEndDraw();
}

void Menu::GameOverMenu::on(int key)
{
}
