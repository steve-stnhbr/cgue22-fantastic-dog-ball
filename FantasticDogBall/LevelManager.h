#pragma once

#include "Level.h"
#include "HUD.h"
#include "State.h"

class LevelManager
{
private:
public:
	static State state;

	static Level* current;
	static Player* playerTemplate;
	std::vector<Level*> levels;

	LevelManager();

	void load(unsigned short);
	void render();
};

