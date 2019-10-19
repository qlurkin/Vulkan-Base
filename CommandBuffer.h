#pragma once

#include <vulkan/vulkan.h>

#include <vector>

class GraphicsPipeline;
class VertexBuffer;
class IndexBuffer;
class UniformBuffer;

class CommandBuffer {
	public:
	CommandBuffer(VertexBuffer vertexBuffers[], size_t vertexBuffersCount, IndexBuffer* indexBuffer, UniformBuffer* uniformBuffer, GraphicsPipeline* pipeline);
	CommandBuffer(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, UniformBuffer* uniformBuffer, GraphicsPipeline* pipeline);
	~CommandBuffer();
	VkCommandBuffer operator[](std::size_t i);

	private:
	GraphicsPipeline* pipeline;
	std::vector<VkCommandBuffer> commandBuffers;
};

#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"