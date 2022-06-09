#pragma once

#include "Level.h"
#include "HUD.h"
#include "State.h"

class LevelManager
{
private:
public:
	State state = State::PLAYING;

	static Level* current;
	static Player* playerTemplate;
	std::vector<Level*> levels;

	LevelManager();

	void load(unsigned short);
	void render();
};

