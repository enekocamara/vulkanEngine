#include "first_app.h"
#include "simple_render_system.h"

#include <stdexcept>
#include <vector>
#include <array>

namespace mve {

	FirstApp::FirstApp() {
		loadGameObjects();
	}

	FirstApp::~FirstApp() {
	}

	void FirstApp::run() {
		SimpleRenderSystem simpleRenderSystem{mveDevice, mveRenderer.getSwapChainRenderPass()};
		while (!mveWindow.shouldClose())
		{
			glfwPollEvents();
			if (auto commandBuffer = mveRenderer.beginFrame()) {
				mveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
				mveRenderer.endSwapChainRenderPass(commandBuffer);
				mveRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(mveDevice.device());
	}

	void FirstApp::makeTriangle(std::vector<MveModel::Vertex>& vertices,
			glm::vec2 left,
			glm::vec2 right,
			glm::vec2 up) {
		vertices.push_back({ { up },{ 1.0f, 0.0f, 0.0f} });
		vertices.push_back({ { right },{ 0.0f, 1.0f, 0.0f} });
		vertices.push_back({ { left },{ 0.0f, 0.0f, 1.0f} });
	}

	void FirstApp::sierpinski(std::vector<MveModel::Vertex>& vertices,
		glm::vec2 left, glm::vec2 right, glm::vec2 up, int depth) {
		if (depth <= 0)
			makeTriangle(vertices, left, right, up);
		else {
			auto leftUp = 0.5f * (left + up);
			auto rightUp = 0.5f * (right + up);
			auto leftRight = 0.5f * (left + right);
			sierpinski(vertices, left, leftRight, leftUp, depth - 1);
			sierpinski(vertices, leftRight, right, rightUp, depth - 1);
			sierpinski(vertices, leftUp, rightUp, up, depth - 1);
		}
	}

	void FirstApp::grid(std::vector<MveModel::Vertex>& vertices, size_t depth) {
		/*for (size_t j = 0; j < depth; j++) {
			for (size_t i = 0; i < depth; i++) {
				makeTriangle(vertices, left, right, up);
			}
		}*/
	}

	void FirstApp::loadGameObjects() {
		std::vector<MveModel::Vertex> vertices;
		explodeTriangles(vertices);
	}

	void FirstApp::explodeTriangles(std::vector<MveModel::Vertex>& vertices) {
		sierpinski(vertices, { 0.2f, 0.2f }, { -0.2f, 0.2f }, { 0.0f, -0.2f }, 11);

		auto mveModel = std::make_shared<MveModel>(mveDevice, vertices);

		for (int i = 0; i < 10; i++) {
			auto triangle = MveGameObject::createGameObject();
			triangle.model = mveModel;
			triangle.color = { .1f, 0.1f + static_cast<float>(i) / 10.0f , .1f };
			triangle.transform2d.translation.x = .0f;
			triangle.transform2d.scale = { 1.f, 1.f };// { 1.f + static_cast<float>(i) / 5.0f, 1.f + static_cast<float>(i) / 5.0f };
			triangle.transform2d.rotation = 0.0f;
			gameObjects.push_back(std::move(triangle));
		}
	}

	void FirstApp::gravity(std::vector<MveModel::Vertex>& vertices) {
		
		grid(vertices, 10);

		auto mveModel = std::make_shared<MveModel>(mveDevice, vertices);

		for (int i = 0; i < 10; i++) {
			auto triangle = MveGameObject::createGameObject();
			triangle.model = mveModel;
			triangle.color = { .1f, 0.1f + static_cast<float>(i) / 10.0f , .1f };
			triangle.transform2d.translation.x = .0f;
			triangle.transform2d.scale = { 1.f, 1.f };// { 1.f + static_cast<float>(i) / 5.0f, 1.f + static_cast<float>(i) / 5.0f };
			triangle.transform2d.rotation = 0.0f;
			gameObjects.push_back(std::move(triangle));
		}
	}
}