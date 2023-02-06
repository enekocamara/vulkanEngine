#include "first_app.h"
#include "simple_render_system.h"
#include "gravity.h"
#include "geometry.h"

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

	void FirstApp::loadGameObjects() {
		std::shared_ptr<MveModel> mveModel = createCubeModel(mveDevice, glm::vec3{ .0f });

		auto cube = MveGameObject::createGameObject();
		cube.model = mveModel;
		cube.transform.translation = { .0f, .0f, .5f };
		cube.transform.scale = { .5f, .5f, .5f };
		gameObjects.push_back(std::move(cube));
	}
	/*
	void FirstApp::gravity() {
		// create some models
		std::shared_ptr<MveModel> squareModel = Geometry::createSquareModel(mveDevice,{ .5f, .0f });
		std::shared_ptr<MveModel> circleModel = Geometry::createCircleModel(mveDevice, 70);
		
		//physic objects

		auto red = MveGameObject::createGameObject();
		red.color = { 1.0f, 0.0f, 0.0f };
		red.model = circleModel;
		red.transform2d.scale = { 0.005f, 0.005f };
		red.transform2d.translation = { .5f, .5f };
		red.rigidBody2d.mass = 10;
		red.rigidBody2d.velocity = { -.5f, .0f };
		physicsObjects.push_back(std::move(red));
		auto blue = MveGameObject::createGameObject();

		blue.color = { 0.0f, 0.0f, 1.0f };
		blue.model = circleModel;
		blue.transform2d.scale = { 0.05f, 0.05f };
		blue.transform2d.translation = { -.45f, -.25f };
		blue.rigidBody2d.mass = 10;
		blue.rigidBody2d.velocity = { .5f, .0f };

		
		physicsObjects.push_back(std::move(blue));

		//grabity field
		int gridCount = 20;
		for (int i = 0; i < gridCount; i++) {
			for (int j = 0; j < gridCount; j++) {
				auto vf = MveGameObject::createGameObject();
				vf.transform2d.scale = glm::vec2(0.005f);
				vf.transform2d.translation = {
					-1.0f + (i + 0.5f) * 2.0f / gridCount,
					-1.0f + (j + 0.5f) * 2.0f / gridCount };
				vf.color = glm::vec3(1.0f);
				vf.model = squareModel;
				vectorField.push_back(std::move(vf));
			}
		}		
	}	*/

	std::unique_ptr<MveModel> FirstApp::createCubeModel(MveDevice& device, glm::vec3 offset) {
		std::vector<MveModel::Vertex> vertices{

			// left face (white)
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

		};
		for (auto& v : vertices) {
			v.position += offset;
		}
		return std::make_unique<MveModel>(device, vertices);
	}
}