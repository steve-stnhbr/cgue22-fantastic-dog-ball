#pragma once

#include "Level.h"
#include "HUD.h"
#include "State.h"
#include "Menu.h"

class LevelManager
{
private:
public:
	static State state;

	static Level* current;
	static unsigned currentIndex;
	static Player* playerTemplate;
	static std::vector<Level*> levels;

	Menu::StartMenu*	startMenu;
	Menu::TimeOverMenu* timeOverMenu;
	Menu::GameOverMenu* gameOverMenu;
	Menu::FinishedMenu* finishedMenu;

	LevelManager();

	static void load(unsigned short);
	void render();
};

