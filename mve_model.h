#pragma once

#include "mve_device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace mve {
class MveModel
{

public:
	struct Vertex {
		glm::vec2 position;
		glm::vec3 color;

		static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	};
	MveModel(MveDevice& device, const std::vector<Vertex>& vertices);
	~MveModel();

	MveModel(const MveModel&) = delete;
	MveModel& operator=(const MveModel&) = delete;
	
	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);

private:
	void createVertexBuffers(const std::vector<Vertex>& vertices);

	MveDevice& mveDevice;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	uint32_t vertexCount;
};

}
