#include "DescriptorSet.h"

DescriptorSet::DescriptorSet(DescriptorSetLayout& descriptorSetLayout, Engine* engine) : engine(engine), descriptorSetLayout(descriptorSetLayout) {
	std::vector<VkDescriptorSetLayout> layouts(engine->getSwapChainBuffersCount(), descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = engine->getDescriptorPool();
	allocInfo.descriptorSetCount = static_cast<uint32_t>(engine->getSwapChainBuffersCount());
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(engine->getSwapChainBuffersCount());
	if (vkAllocateDescriptorSets(engine->getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	LOG << "DescriptorSet Created" << ENDL;
}

DescriptorSet& DescriptorSet::setUniformBuffer(UniformBuffer& uniformBuffer, uint32_t binding) {
	LOG << "Yop1" << ENDL;

	for (size_t i = 0; i < engine->getSwapChainBuffersCount(); i++) {
		
		VkDescriptorBufferInfo bufferInfo = {};
		LOG << "YopIn" << ENDL;
		bufferInfo.buffer = uniformBuffer[i];
		LOG << "YopIn" << ENDL;
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

	LOG << "Yop2" << ENDL;

	return *this;
}

DescriptorSet& DescriptorSet::setTexture(Texture& texture, uint32_t binding) {

}

DescriptorSet::~DescriptorSet() {
	LOG << "DescriptorSet Destroyed" << ENDL;
}

VkDescriptorSet DescriptorSet::operator[](std::size_t i) {
	return descriptorSets[i];
}