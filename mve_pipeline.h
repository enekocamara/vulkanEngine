#pragma once

#include "mve_device.h"

#include <string>
#include <vector>

namespace mve {
	struct PipelineConfigInfo{
		//PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		//PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
		//PipelineConfigInfo() = default;

		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class MvePipeline
	{
	public:
		MvePipeline(MveDevice& device,
				const std::string& vertFilePath,
				const std::string& fragFilePath,
				const PipelineConfigInfo configInfo);
		~MvePipeline();

		MvePipeline(const MvePipeline&) = delete;
		MvePipeline& operator=(const MvePipeline&) = delete;
		

		void bind(VkCommandBuffer commandBuffer);
		
		static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
	private:
		static std::vector<char> readFile(const std::string& filePath);
		
		void createGraphicsPipeline(
			const std::string& vertFilePath,
			const std::string& fragFilePath,
			const PipelineConfigInfo& configInfo);
		
		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		MveDevice& mveDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}
