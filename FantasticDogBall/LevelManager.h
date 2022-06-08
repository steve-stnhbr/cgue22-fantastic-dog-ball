#pragma once

#include "Level.h"
#include "HUD.h"

class LevelManager
{
private:
public:
	enum State {
		START_MENU,
		PLAYING,
		GAME_OVER,
		TIME_OVER,
		LEVEL_FINISHED
	} state = PLAYING;

	static Level* current;
	static Player* playerTemplate;
	std::vector<Level*> levels;

	LevelManager();

	void load(unsigned short);
	void render();
};

