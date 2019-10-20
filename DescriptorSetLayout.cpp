#include "DescriptorSetLayout.h"

DescriptorSetLayout::DescriptorSetLayout(Engine* engine) : engine(engine) {
	ended = false;
}

DescriptorSetLayout::~DescriptorSetLayout() {
	vkDestroyDescriptorSetLayout(engine->getDevice(), descriptorSetLayout, nullptr);
	LOG << "DescriptorSetLayout Destroyed" << ENDL;
}

DescriptorSetLayout& DescriptorSetLayout::addUniform() {
	if(ended) {
		LOG << "Adding Uniform to Ended DescriptorSet > IGNORED" << ENDL;
		return *this;
	}

	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = bindings.size();
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	bindings.push_back(uboLayoutBinding);

	return *this;
}

DescriptorSetLayout& DescriptorSetLayout::addTexture() {
	if(ended) {
		LOG << "Adding Texture to Ended DescriptorSet > IGNORED" << ENDL;
		return *this;
	}

	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = bindings.size();
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	bindings.push_back(samplerLayoutBinding);

	return *this;
}

void DescriptorSetLayout::end() {
	ended = true;

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(engine->getDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}

	LOG << "DescriptorSetLayout Created" << ENDL;
}

DescriptorSetLayout::operator VkDescriptorSetLayout() {
	return descriptorSetLayout;
}