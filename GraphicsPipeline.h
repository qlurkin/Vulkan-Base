#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Engine;
class VertexInputState;
class Shader;
class CommandBuffer;
class VertexBuffer;
class CommandPool;
class DescriptorSetLayout;

#include "SwapChain.h"

class GraphicsPipeline : public SwapChainObserver {
	public:
	GraphicsPipeline(VertexInputState vertexInputState, Shader& vertexShader, Shader& fragmentShader, DescriptorSetLayout& descriptorSetLayout, Engine* engine);
	~GraphicsPipeline();
	operator VkPipeline();
	Engine* getEngine();
	VkPipelineLayout getPipelineLayout();
	virtual void cleanup();
	virtual void creation();

	private:
	Engine* engine;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
};

#include "Engine.h"