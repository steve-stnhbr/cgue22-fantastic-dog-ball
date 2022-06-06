#pragma once
#include <GLFW/glfw3.h>

class Inputs {
public:
	struct Processor {
		virtual void onW() = 0;
		virtual void onA() = 0;
		virtual void onS() = 0;
		virtual void onD() = 0;
	};

	inline static Processor* processor = nullptr;

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (processor == nullptr) return;
		if (action == GLFW_PRESS) {
			switch (key) {
			case GLFW_KEY_W:
				processor->onW();
				break;
			case GLFW_KEY_A:
				processor->onA();
				break;
			case GLFW_KEY_S:
				processor->onS();
				break;
			case GLFW_KEY_D:
				processor->onD();
				break;
			}
		}
	}

	static inline void setProcessor(Processor* processor_) {
		processor = processor_;
	}
};