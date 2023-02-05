#pragma once

#include "mve_device.h"
#include "mve_game_object.h"
#include "mve_window.h"
#include "mve_renderer.h"

#include <memory>
#include <vector>
namespace mve {

class FirstApp
{
public:
	static constexpr int HEIGHT = 1080;
	static constexpr int WIDTH = 1920;

	FirstApp();
	~FirstApp();

	FirstApp(const FirstApp&) = delete;
	FirstApp& operator=(const FirstApp&) = delete;
	void run();
private:
	void makeTriangle(std::vector<MveModel::Vertex>& vertices,
		glm::vec2 left,
		glm::vec2 right,
		glm::vec2 up);
	void sierpinski(std::vector<MveModel::Vertex>& vertices,
		glm::vec2 left,
		glm::vec2 right,
		glm::vec2 up,
		int depth);
	void grid(std::vector<MveModel::Vertex>& vertices, size_t depht);
	void gravity(std::vector<MveModel::Vertex>& vertices);
	void explodeTriangles(std::vector<MveModel::Vertex>& vertices);
	void loadGameObjects();

	MveWindow mveWindow{WIDTH, HEIGHT, "Hello Vulkan"};
	MveDevice mveDevice{mveWindow};
	MveRenderer mveRenderer{mveWindow, mveDevice};
	std::vector<MveGameObject> gameObjects;
};

}