#pragma once

#include <vulkan/vulkan.h>

class Engine;

class VertexBuffer {
	public:
	VertexBuffer(void* dataStart, size_t vertexCount, VkDeviceSize vertexSize, Engine* engine);
	~VertexBuffer();
	operator VkBuffer();

	private:
	size_t count;
	Engine* engine;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
};

#include "Engine.h"