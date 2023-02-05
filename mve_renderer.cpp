#include "mve_renderer.h"

#include <stdexcept>
#include <vector>
#include <array>

namespace mve {

	MveRenderer::MveRenderer(MveWindow& window, MveDevice& device) : mveWindow{window}, mveDevice{device} {
		recreateSwapChain();
		createCommandBuffers();
	}

	MveRenderer::~MveRenderer() {
		freeCommandBuffers();
	}

	void MveRenderer::recreateSwapChain() {
		auto extent = mveWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = mveWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(mveDevice.device());

		if (mveSwapChain == nullptr) {
			mveSwapChain = std::make_unique<MveSwapChain>(mveDevice, extent);
		}
		else {
			std::shared_ptr<MveSwapChain> oldSwapChain = std::move(mveSwapChain);
			mveSwapChain = std::make_unique<MveSwapChain>(mveDevice, extent, oldSwapChain);
			if (!oldSwapChain->compareSwapFormats(*mveSwapChain.get())) {
				throw std::runtime_error("Swap chain image(or depth) format has changed");
			}
		}
	}

	void MveRenderer::createCommandBuffers() {
		commandBuffers.resize(MveSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = mveDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(mveDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
			throw std::runtime_error("failed to create command buffer");
	}

	void MveRenderer::freeCommandBuffers() {
		vkFreeCommandBuffers(
			mveDevice.device(),
			mveDevice.getCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()),
			commandBuffers.data());
		commandBuffers.clear();
	}

	VkCommandBuffer MveRenderer::beginFrame() {
		assert(!isFrameStarted && "Cant call beginFrame while already in progres");

		auto result = mveSwapChain->acquireNextImage(&currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return nullptr;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("failed to aquire swap chain image");
		isFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
			throw std::runtime_error("failed to begin recording command buffer");
		return commandBuffer;
	}

	void MveRenderer::endFrame() {
		assert(isFrameStarted && "can't call endFrame while frame is not in progress");
		auto commandBuffer = getCurrentCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			throw std::runtime_error("failed to record command buffer");

		auto result = mveSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
			mveWindow.wasWindowResized()) {
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS)
			throw std::runtime_error("failed to aquire swap chain image");
		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % MveSwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void MveRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer){
		assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on a commandBuffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = mveSwapChain->getRenderPass();
		renderPassInfo.framebuffer = mveSwapChain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = mveSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2>clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewPort{};
		viewPort.x = 0.0f;
		viewPort.y = 0.0f;
		viewPort.width = static_cast<uint32_t>(mveSwapChain->getSwapChainExtent().width);
		viewPort.height = static_cast<uint32_t>(mveSwapChain->getSwapChainExtent().height);
		viewPort.minDepth = 0.0f;
		viewPort.maxDepth = 1.0f;
		VkRect2D scissor{ { 0,0 }, mveSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewPort);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}
	void MveRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer){
	
		assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on a commandBuffer from a different frame");
		vkCmdEndRenderPass(commandBuffer);
	}

}