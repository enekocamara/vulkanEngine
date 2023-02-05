#pragma once

#include "mve_device.h"
#include "mve_game_object.h"
#include "mve_pipeline.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <memory>
#include <vector>
namespace mve {
	class SimpleRenderSystem
	{
		struct SimplePushConstantData {
			glm::mat2 transform{ 1.f };
			glm::vec2 offset;
			alignas(16)glm::vec3 color;
		};
	public:

		SimpleRenderSystem(MveDevice& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;
		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<MveGameObject>& gameObjects);

		void explodingTriangle(VkCommandBuffer commandBuffer, std::vector<MveGameObject>& gameObjects);
		void gravityField(VkCommandBuffer commandBuffer, std::vector<MveGameObject>& gameObjects);
	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		MveDevice& mveDevice;

		std::unique_ptr<MvePipeline> mvePipeline;
		VkPipelineLayout pipelineLayout;
	};

}