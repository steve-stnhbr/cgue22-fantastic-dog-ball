#pragma once

#include "Level.h"

class LevelManager
{
private:
public:
	static Level* current;
	std::vector<Level*> levels;

	LevelManager();

	void load(unsigned short);
	void render();
};

