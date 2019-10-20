#include "UniformBuffer.h"
#include "buffers.h"

UniformBuffer::UniformBuffer(VkDeviceSize bufferSize, GraphicsPipeline* pipeline) : engine(pipeline->getEngine()), bufferSize(bufferSize) {
	uniformBuffers.resize(engine->getSwapChainBuffersCount());
	uniformBuffersMemory.resize(engine->getSwapChainBuffersCount());

	for (size_t i = 0; i < engine->getSwapChainBuffersCount(); i++) {
		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i], engine);
	}
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

VkDeviceSize UniformBuffer::getSize() {
	return bufferSize;
}