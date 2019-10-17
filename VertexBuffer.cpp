#include "VertexBuffer.h"
#include "Log.h"
#include "buffers.h"
#include <cstring>

VertexBuffer::VertexBuffer(void* dataStart, size_t vertexCount, VkDeviceSize vertexSize, Engine* engine) : engine(engine), count(vertexCount) {
	VkDeviceSize size = vertexCount * vertexSize;
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, engine);

	void* data;
	vkMapMemory(engine->getDevice(), stagingBufferMemory, 0, size, 0, &data);
		memcpy(data, dataStart, (size_t) size);
	vkUnmapMemory(engine->getDevice(), stagingBufferMemory);

	createBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory, engine);

	copyBuffer(stagingBuffer, vertexBuffer, size, engine);

	vkDestroyBuffer(engine->getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(engine->getDevice(), stagingBufferMemory, nullptr);

	LOG << "VertexBuffer Created" << ENDL;
}

VertexBuffer::~VertexBuffer() {
	vkDestroyBuffer(engine->getDevice(), vertexBuffer, nullptr);
	vkFreeMemory(engine->getDevice(), vertexBufferMemory, nullptr);
	LOG << "VertexBuffer Destroyed" << ENDL;
}

VertexBuffer::operator VkBuffer() {
	return vertexBuffer;
}