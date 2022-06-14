#pragma once
#include <GLFW/glfw3.h>

class Inputs {
public:
	struct Processor {
		virtual void on(int key) {};
		virtual void off(int key) {};
	};

	inline static Processor* processor = nullptr;

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		return;
		if (processor == nullptr) return;
		if (action == GLFW_PRESS) {
			processor->on(key);
		}
		else if (action == GLFW_RELEASE) {
			processor->off(key);
		}
	}

	static inline void setProcessor(Processor* processor_) {
		processor = processor_;
	}
};