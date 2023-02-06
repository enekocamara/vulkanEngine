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
	
	static void makeRectangle(std::vector<MveModel::Vertex>& vertices,
		glm::vec2 left,
		glm::vec2 right,
		glm::vec2 up);
private:
	
	void grid(std::vector<MveModel::Vertex>& vertices, size_t depht);
	void gravity();
	void explodeTriangles(std::vector<MveModel::Vertex>& vertices);
	void loadGameObjects();
	std::unique_ptr<MveModel> createCubeModel(MveDevice& device, glm::vec3 offset);

	MveWindow mveWindow{WIDTH, HEIGHT, "Hello Vulkan"};
	MveDevice mveDevice{mveWindow};
	MveRenderer mveRenderer{mveWindow, mveDevice};
	std::vector<MveGameObject> gameObjects;
	std::vector<MveGameObject> physicsObjects;
	std::vector<MveGameObject> vectorField;
};

}