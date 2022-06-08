#pragma once
#include "Inputs.h"

namespace Menu {
	class Button {
		enum State {
			IDLE,
			HOVER,
			CLICK
		};
		
		unsigned centerX, centerY, width, height;

	};
	class StartMenu : Inputs::Processor {

	};
	class TimeOverMenu : Inputs::Processor {

	};
	class GameOverMenu : Inputs::Processor {

	};
}