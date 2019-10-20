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

class GraphicsPipeline {
	public:
	GraphicsPipeline(VertexInputState vertexInputState, Shader& vertexShader, Shader& fragmentShader, DescriptorSetLayout& descriptorSetLayout, Engine* engine);
	~GraphicsPipeline();
	operator VkPipeline();
	Engine* getEngine();
	VkPipelineLayout getPipelineLayout();

	private:
	Engine* engine;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
};

#include "Engine.h"