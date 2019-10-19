#include "UniformBuffer.h"
#include "buffers.h"

UniformBuffer::UniformBuffer(VkDeviceSize bufferSize, GraphicsPipeline* pipeline) : engine(pipeline->getEngine()), bufferSize(bufferSize) {
	uniformBuffers.resize(engine->getSwapChainBuffersCount());
	uniformBuffersMemory.resize(engine->getSwapChainBuffersCount());

	for (size_t i = 0; i < engine->getSwapChainBuffersCount(); i++) {
		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i], engine);
	}

	createDescriptorSets(pipeline->getDescriptorSetLayout());
}

UniformBuffer::~UniformBuffer() {
	for (size_t i = 0; i < engine->getSwapChainBuffersCount(); i++) {
		vkDestroyBuffer(engine->getDevice(), uniformBuffers[i], nullptr);
		vkFreeMemory(engine->getDevice(), uniformBuffersMemory[i], nullptr);
	}
}

void UniformBuffer::setData(void* dataStart) {
	void* data;
	vkMapMemory(engine->getDevice(), uniformBuffersMemory[engine->getCurrentImage()], 0, bufferSize, 0, &data);
		memcpy(data, dataStart, bufferSize);
	vkUnmapMemory(engine->getDevice(), uniformBuffersMemory[engine->getCurrentImage()]);
}

VkBuffer UniformBuffer::operator[](std::size_t i) {
	return uniformBuffers[i];
}

void UniformBuffer::createDescriptorSets(VkDescriptorSetLayout descriptorSetLayout) {
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

	for (size_t i = 0; i < engine->getSwapChainBuffersCount(); i++) {
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = bufferSize;

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(engine->getDevice(), 1, &descriptorWrite, 0, nullptr);
	}
}

std::vector<VkDescriptorSet>& UniformBuffer::getDescriptorSets() {
	return descriptorSets;
}