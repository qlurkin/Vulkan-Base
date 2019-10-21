#include "CommandBuffer.h"

CommandBuffer::CommandBuffer(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, DescriptorSet* descriptorSet, GraphicsPipeline* pipeline) : CommandBuffer(vertexBuffer, 1, indexBuffer, descriptorSet, pipeline) {}

CommandBuffer::CommandBuffer(VertexBuffer vertexBuffers[], size_t vertexBuffersCount, IndexBuffer* indexBuffer, DescriptorSet* descriptorSet, GraphicsPipeline* pipeline) : pipeline(pipeline), descriptorSet(descriptorSet) {
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

		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
		clearValues[1].depthStencil = {1.0f, 0};

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		VkBuffer legacyVertexBuffers[vertexBuffersCount];
		for(size_t j = 0; j < vertexBuffersCount; j++) {
			legacyVertexBuffers[j] = vertexBuffers[j];
		}

		VkDescriptorSet legacyDescriptorSet = (*descriptorSet)[i];

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline);
			VkDeviceSize offsets[] = {0};
			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, legacyVertexBuffers, offsets);
			vkCmdBindIndexBuffer(commandBuffers[i], *indexBuffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipelineLayout(), 0, 1, &legacyDescriptorSet, 0, nullptr);
			vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indexBuffer->getCount()), 1, 0, 0, 0);
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