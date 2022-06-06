#pragma once

#include "Level.h"

class LevelManager
{
private:
	Level* current;
public:
	std::vector<Level*> levels;

	LevelManager();

	void load(unsigned short);
	void render();
};

