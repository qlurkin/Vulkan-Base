#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Engine;

class DescriptorSetLayout {
	public:
	DescriptorSetLayout(Engine* engine);
	~DescriptorSetLayout();
	DescriptorSetLayout& addUniform();
	DescriptorSetLayout& addTexture();
	void end();
	operator VkDescriptorSetLayout();

	private:
	Engine* engine;
	bool ended;
	VkDescriptorSetLayout descriptorSetLayout;
	std::vector<VkDescriptorSetLayoutBinding> bindings;
};

#include "Engine.h"