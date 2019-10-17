#include "CommandBuffer.h"

CommandBuffer::CommandBuffer(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, GraphicsPipeline* pipeline) : CommandBuffer(vertexBuffer, 1, indexBuffer, pipeline) {}

CommandBuffer::CommandBuffer(VertexBuffer vertexBuffers[], size_t vertexBuffersCount, IndexBuffer* indexBuffer, GraphicsPipeline* pipeline) : pipeline(pipeline) {
	Engine* engine = pipeline->getEngine();
	commandBuffers.resize(engine->getSwapChainBuffersCount());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = engine->getCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

	if (vkAllocateCommandBuffers(engine->getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	for (size_t i = 0; i < commandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = engine->getRenderPass();
		renderPassInfo.framebuffer = engine->getSwapChainBuffer(i);
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = engine->getExtent();
		VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline);
		
		VkBuffer legacyVertexBuffers[vertexBuffersCount];
		for(size_t j = 0; j < vertexBuffersCount; j++) {
			legacyVertexBuffers[j] = vertexBuffers[j];
		}

		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, legacyVertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffers[i], *indexBuffer, 0, VK_INDEX_TYPE_UINT16);
		
		//vkCmdDraw(commandBuffers[i], vertex, 1, 0, 0);
		vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indexBuffer->getCount()), 1, 0, 0, 0);
		//vkCmdDrawIndexed(commandBuffers[i], 6, 1, 0, 0, 0);
		vkCmdEndRenderPass(commandBuffers[i]);
		
		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
	LOG << "CommandBuffer Created" << ENDL;
}

CommandBuffer::~CommandBuffer() {
	Engine* engine = pipeline->getEngine();
	vkFreeCommandBuffers(engine->getDevice(), engine->getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	LOG << "CommandBuffer Destroyed" << ENDL;
}

VkCommandBuffer CommandBuffer::operator[](std::size_t i) {
	return commandBuffers[i];
}