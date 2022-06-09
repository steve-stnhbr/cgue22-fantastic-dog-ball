#pragma once
#include <GLFW/glfw3.h>

class Inputs {
public:
	struct Processor {
		virtual void pressW() {}
		virtual void pressA() {}
		virtual void pressS() {}
		virtual void pressD() {}
		virtual void releaseW() {}
		virtual void releaseA() {}
		virtual void releaseS() {}
		virtual void releaseD() {}
	};

	inline static Processor* processor = nullptr;

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (processor == nullptr) return;
		if (action == GLFW_PRESS) {
			switch (key) {
			case GLFW_KEY_W:
				processor->pressW();
				break;
			case GLFW_KEY_A:
				processor->pressA();
				break;
			case GLFW_KEY_S:
				processor->pressS();
				break;
			case GLFW_KEY_D:
				processor->pressD();
				break;
			}
		}
		else if (action == GLFW_RELEASE) {
			switch (key) {
			case GLFW_KEY_W:
				processor->releaseW();
				break;
			case GLFW_KEY_A:
				processor->releaseA();
				break;
			case GLFW_KEY_S:
				processor->releaseS();
				break;
			case GLFW_KEY_D:
				processor->releaseD();
				break;
			}
		}
	}

	static inline void setProcessor(Processor* processor_) {
		processor = processor_;
	}
};