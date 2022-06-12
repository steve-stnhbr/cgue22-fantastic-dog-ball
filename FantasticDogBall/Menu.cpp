#include "Menu.h"

#include "LevelManager.h"

const float headingScale = 5;
const float textScale = 1.8;
const float textOffset = 100;

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
	const auto x = Globals::WINDOW_WIDTH / 2;
	const auto y = Globals::WINDOW_HEIGHT / 2;
	gltBeginDraw();
	gltColor(1.f, .36f, 1.f, 1.f);
	gltDrawText2DAligned(heading, x, y, headingScale, GLT_CENTER, GLT_CENTER);
	gltColor(1.f, 1.f, 1.f, 1.f);
	gltDrawText2DAligned(text, x, y + textOffset, textScale, GLT_CENTER, GLT_CENTER);
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
	const auto x = Globals::WINDOW_WIDTH / 2;
	const auto y = Globals::WINDOW_HEIGHT / 2;
	gltBeginDraw();
	gltColor(.2f, 1.f, 1.f, 1.f);
	gltDrawText2DAligned(heading, x, y, headingScale, GLT_CENTER, GLT_CENTER);
	gltColor(1.f, 1.f, 1.f, 1.f);
	gltDrawText2DAligned(text, x, y + textOffset, textScale, GLT_CENTER, GLT_CENTER);
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
	const auto x = Globals::WINDOW_WIDTH / 2;
	const auto y = Globals::WINDOW_HEIGHT / 2;
	gltBeginDraw();
	gltColor(1.f, .12f, .35f, 1.f);
	gltDrawText2DAligned(heading, x, y, headingScale, GLT_CENTER, GLT_CENTER);
	gltColor(1.f, 1.f, 1.f, 1.f);
	gltDrawText2DAligned(text, x, y + textOffset, textScale, GLT_CENTER, GLT_CENTER);
	gltEndDraw();
}

void Menu::GameOverMenu::on(int key)
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

Menu::FinishedMenu::FinishedMenu()
{
	gltInit();
	heading = gltCreateText();
	gltSetText(heading, "Congrats!");
	text = gltCreateText();
}

void Menu::FinishedMenu::draw(unsigned time, unsigned score)
{
	const auto x = Globals::WINDOW_WIDTH / 2;
	const auto y = Globals::WINDOW_HEIGHT / 2;
	if (LevelManager::currentIndex == Globals::NUM_LEVELS - 1) {
		gltSetText(text, Utils::string_format("You finished level %i in %i seconds with %i bones\npress r: replay level\npress e: return to start",
			LevelManager::currentIndex + 1,
			time,
			score).c_str());
	}
	else {
		gltSetText(text, Utils::string_format("You finished level %i in %i seconds with %i bones\npress n: next level\npress r: replay level\npress e: return to start",
			LevelManager::currentIndex + 1,
			time,
			score).c_str());
	}

	gltBeginDraw();
	gltColor(.1f, 1.f, .3f, 1.f);
	gltDrawText2DAligned(heading, x, y, headingScale, GLT_CENTER, GLT_CENTER);
	gltColor(1.f, 1.f, 1.f, 1.f);
	gltDrawText2DAligned(text, x, y + textOffset, textScale, GLT_CENTER, GLT_CENTER);
	gltEndDraw();
}

void Menu::FinishedMenu::on(int key)
{
	switch (key) {
	case GLFW_KEY_E:
		LevelManager::state = State::START_MENU;
		break;
	case GLFW_KEY_R:
		LevelManager::load(LevelManager::currentIndex);
		break;
	case GLFW_KEY_N:
		LevelManager::load(LevelManager::currentIndex + 1);
		break;
	}
}
