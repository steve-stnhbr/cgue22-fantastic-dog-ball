#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLT_IMPLEMENTATION
#include <gltext.h>
#include "Inputs.h"
#include "Utils.h"

namespace Menu {
	class Button {
		enum State {
			IDLE,
			HOVER,
			CLICK
		};
		
		unsigned centerX, centerY, width, height;

	};
	class StartMenu : public Inputs::Processor {
	private:
		GLTtext* heading, * text;
	public:
		StartMenu();

		void draw();
		void on(int key) override;
	};
	class FinishedMenu : public Inputs::Processor {
	private:
		GLTtext* heading, *text;
	public:
		FinishedMenu();

		void draw(unsigned time, unsigned score);
		void on(int key) override;
	};
	class TimeOverMenu : public Inputs::Processor {
	private:
		GLTtext* heading, *text;
	public:
		TimeOverMenu();

		void draw();
		void on(int key) override;
	};
	class GameOverMenu : public Inputs::Processor {
	private:
		GLTtext* heading, *text;
	public:
		GameOverMenu();

		void draw();
		void on(int key) override;
	};
}