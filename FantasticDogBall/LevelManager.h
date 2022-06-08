#pragma once

#include "Level.h"
#include "HUD.h"

class LevelManager
{
private:
public:
	static Level* current;
	static HUD* hud;
	std::vector<Level*> levels;

	LevelManager();

	void load(unsigned short);
	void render();

	static void start2D();
	static void end2D();
};

