#include "IndexBuffer.h"
#include "buffers.h"

IndexBuffer::IndexBuffer(void* dataStart, size_t indexCount, VkDeviceSize indexSize, Engine* engine) : engine(engine), count(indexCount) {
	VkDeviceSize size = indexCount * indexSize;
	
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, engine);

	void* data;
	vkMapMemory(engine->getDevice(), stagingBufferMemory, 0, size, 0, &data);
	memcpy(data, dataStart, (size_t) size);
	vkUnmapMemory(engine->getDevice(), stagingBufferMemory);

	createBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory, engine);

	copyBuffer(stagingBuffer, indexBuffer, size, engine);

	vkDestroyBuffer(engine->getDevice(), stagingBuffer, nullptr);
	vkFreeMemory(engine->getDevice(), stagingBufferMemory, nullptr);

	LOG << "IndexBuffer Created" << ENDL;
}

IndexBuffer::~IndexBuffer() {
	vkDestroyBuffer(engine->getDevice(), indexBuffer, nullptr);
	vkFreeMemory(engine->getDevice(), indexBufferMemory, nullptr);

	LOG << "IndexBuffer Destroyed" << ENDL;
}

IndexBuffer::operator VkBuffer() {
	return indexBuffer;
}

size_t IndexBuffer::getCount() {
	return count;
}