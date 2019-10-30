#include "DescriptorSet.h"

DescriptorSet::DescriptorSet(DescriptorSetLayout& descriptorSetLayout, Engine* engine) : engine(engine), descriptorSetLayout(descriptorSetLayout) {
	SwapChain& swapChain = engine->getSwapChain();
	std::vector<VkDescriptorSetLayout> layouts(swapChain.getSwapChainBuffersCount(), descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = swapChain.getDescriptorPool();
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChain.getSwapChainBuffersCount());
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(swapChain.getSwapChainBuffersCount());
	if (vkAllocateDescriptorSets(engine->getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	LOG << "DescriptorSet Created" << ENDL;
}

DescriptorSet& DescriptorSet::setUniformBuffer(UniformBuffer& uniformBuffer, uint32_t binding) {
	SwapChain& swapChain = engine->getSwapChain();
	for (size_t i = 0; i < swapChain.getSwapChainBuffersCount(); i++) {
		
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffer[i];
		bufferInfo.offset = 0;
		bufferInfo.range = uniformBuffer.getSize();

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(engine->getDevice(), 1, &descriptorWrite, 0, nullptr);
	}

	return *this;
}

DescriptorSet& DescriptorSet::setTexture(Texture& texture, uint32_t binding) { 
	SwapChain& swapChain = engine->getSwapChain();
	for (size_t i = 0; i < swapChain.getSwapChainBuffersCount(); i++) {
		
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = texture.getImageView();
		imageInfo.sampler = texture.getSampler();

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(engine->getDevice(), 1, &descriptorWrite, 0, nullptr);
	}

	return *this;
}

DescriptorSet::~DescriptorSet() {
	LOG << "DescriptorSet Destroyed" << ENDL;
}

VkDescriptorSet DescriptorSet::operator[](std::size_t i) {
	return descriptorSets[i];
}