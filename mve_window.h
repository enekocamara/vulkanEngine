#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <string>

namespace mve {

	class MveWindow
	{
	public:
		MveWindow(int width, int height, std::string windowName);
		~MveWindow();

		MveWindow(const MveWindow&) = delete;
		MveWindow& operator=(const MveWindow&) = delete;

		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

		bool shouldClose() { return glfwWindowShouldClose(window); }
		bool wasWindowResized() { return framebufferResized; }
		void resetWindowResizedFlag() { framebufferResized = false; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	
	private:
		static void framebufferResizeCallBack(GLFWwindow* window, int width, int height);
		void initWindow();
	
		int width;
		int height;
		bool framebufferResized = false;
		std::string windowName;
		GLFWwindow* window;
	};

}