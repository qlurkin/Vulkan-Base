#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Engine;
class DescriptorSetLayout;
class UniformBuffer;
class Texture;

class DescriptorSet {
	public:
	DescriptorSet(DescriptorSetLayout& descriptorSetLayout, Engine* engine);
	~DescriptorSet();
	DescriptorSet& setUniformBuffer(UniformBuffer& uniformBuffer, uint32_t binding);
	DescriptorSet& setTexture(Texture& texture, uint32_t binding);
	VkDescriptorSet operator[](std::size_t i);

	private:
	Engine* engine;
	DescriptorSetLayout& descriptorSetLayout;
	std::vector<VkDescriptorSet> descriptorSets;
};

#include "Engine.h"