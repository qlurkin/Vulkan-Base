#pragma once

#include <vulkan/vulkan.h>

#include <vector>

class GraphicsPipeline;
class VertexBuffer;
class IndexBuffer;
class DescriptorSet;

class CommandBuffer {
	public:
	CommandBuffer(VertexBuffer vertexBuffers[], size_t vertexBuffersCount, IndexBuffer* indexBuffer, DescriptorSet* descriptorSet, GraphicsPipeline* pipeline);
	CommandBuffer(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, DescriptorSet* descriptorSet, GraphicsPipeline* pipeline);
	~CommandBuffer();
	VkCommandBuffer operator[](std::size_t i);

	private:
	DescriptorSet* descriptorSet;
	GraphicsPipeline* pipeline;
	std::vector<VkCommandBuffer> commandBuffers;
};

#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "DescriptorSet.h"