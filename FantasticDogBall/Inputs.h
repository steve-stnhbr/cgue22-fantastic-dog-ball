#pragma once
#include <GLFW/glfw3.h>

#include <map>

class Inputs {
public:
	struct Processor {
		virtual void press(int key) {};
		virtual void pressed(int key) {};
		virtual void release(int key) {};
		virtual void released(int key) {};
	};

	inline static Processor* processor = nullptr;
	inline static std::map<int, int> keyActionMap;

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		return;
		if (processor == nullptr) return;
		if (action == GLFW_PRESS) {
			processor->pressed(key);
			if (keyActionMap[key] == GLFW_RELEASE)
				processor->press(key);
			keyActionMap[key] = action;
		}
		else if (action == GLFW_RELEASE) {
			processor->released(key);
			if (keyActionMap[key] == GLFW_PRESS)
				processor->release(key);
			keyActionMap[key] = action;
		}
	}

	static inline void setProcessor(Processor* processor_) {
		processor = processor_;
	}
};