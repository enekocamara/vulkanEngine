#include "simple_render_system.h"

#include <stdexcept>
#include <vector>
#include <array>

namespace mve {

	SimpleRenderSystem::SimpleRenderSystem(MveDevice& device, VkRenderPass renderPass) 
		: mveDevice{ device }{
		createPipelineLayout();
		createPipeline(renderPass);
	}

	SimpleRenderSystem::~SimpleRenderSystem() {
		vkDestroyPipelineLayout(mveDevice.device(), pipelineLayout, nullptr);
	}

	void SimpleRenderSystem::createPipelineLayout() {

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		createInfo.setLayoutCount = 0;
		createInfo.pSetLayouts = nullptr;
		createInfo.pushConstantRangeCount = 1;
		createInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(mveDevice.device(), &createInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
			throw std::runtime_error("failed to create pipeline layout");
	}
	void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipelineLayout");
		PipelineConfigInfo pipelineConfig{};
		MvePipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;

		mvePipeline = std::make_unique<MvePipeline>(
			mveDevice,
			"shaders/vert.spv",
			"shaders/frag.spv",
			pipelineConfig
			);
	}

	void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<MveGameObject>& gameObjects) {
		mvePipeline->bind(commandBuffer);
		explodingTriangle(commandBuffer, gameObjects);
	}

	void SimpleRenderSystem::explodingTriangle(VkCommandBuffer commandBuffer, std::vector<MveGameObject>& gameObjects) {
		float i = 0;
		for (auto& object : gameObjects) {
			//object.transform2d.rotation = glm::mod(object.transform2d.rotation + 0.1f, glm::two_pi<float>());
			float scale = object.transform2d.scale.x + (i * 0.1f) * i * 0.01;
			object.transform2d.scale.x = scale;
			object.transform2d.scale.y = scale;
			SimplePushConstantData push{};
			push.offset = object.transform2d.translation;
			push.color = object.color;
			push.transform = object.transform2d.mat2();
			vkCmdPushConstants(
				commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);
			object.model->bind(commandBuffer);
			object.model->draw(commandBuffer);
			i++;
		}
	}

	void SimpleRenderSystem::gravityField(VkCommandBuffer commandBuffer, std::vector<MveGameObject>& gameObjects) {
		float i = 0;

		for (auto& object : gameObjects) {
			//object.transform2d.rotation = glm::mod(object.transform2d.rotation + 0.1f, glm::two_pi<float>());
			float scale = object.transform2d.scale.x + (i * 0.1f) * i * 0.01;
			object.transform2d.scale.x = scale;
			object.transform2d.scale.y = scale;
			SimplePushConstantData push{};
			push.offset = object.transform2d.translation;
			push.color = object.color;
			push.transform = object.transform2d.mat2();
			vkCmdPushConstants(
				commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);
			object.model->bind(commandBuffer);
			object.model->draw(commandBuffer);
			i++;
		}
	}
}