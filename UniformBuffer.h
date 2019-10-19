#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Engine;
class GraphicsPipeline;

class UniformBuffer {
	public:
	UniformBuffer(VkDeviceSize bufferSize, GraphicsPipeline* pipeline);
	~UniformBuffer();
	void setData(void* dataStart);
	VkBuffer operator[](std::size_t i);
	std::vector<VkDescriptorSet>& getDescriptorSets();

	private:
	Engine* engine;
	VkDeviceSize bufferSize;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<VkDescriptorSet> descriptorSets;
	void createDescriptorSets(VkDescriptorSetLayout descriptorSetLayout);
};

#include "Engine.h"