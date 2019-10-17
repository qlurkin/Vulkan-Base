#pragma once

#include <vulkan/vulkan.h>

#include <vector>

class VertexInputState {
	public:
	VertexInputState(size_t stride);
	~VertexInputState();
	operator VkPipelineVertexInputStateCreateInfo&();
	VertexInputState& addAttribute(VkFormat format, uint32_t offset);

	private:
	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	VkVertexInputBindingDescription bindingDescription;
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
};