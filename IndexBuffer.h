#pragma once

#include <vulkan/vulkan.h>

class Engine;

class IndexBuffer {
	public:
	IndexBuffer(void* dataStart, size_t indexCount, VkDeviceSize indexSize, Engine* engine);
	~IndexBuffer();
	operator VkBuffer();
	size_t getCount();

	private:
	size_t count;
	Engine* engine;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
};

#include "Engine.h"