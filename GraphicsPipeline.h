#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Engine;
class VertexInputState;
class Shader;
class CommandBuffer;
class VertexBuffer;
class CommandPool;
class UniformBuffer;

class GraphicsPipeline {
	public:
	GraphicsPipeline(VertexInputState vertexInputState, Shader& vertexShader, Shader& fragmentShader, Engine* engine);
	~GraphicsPipeline();
	operator VkPipeline();
	Engine* getEngine();
	VkDescriptorSetLayout getDescriptorSetLayout();
	VkPipelineLayout getPipelineLayout();

	private:
	Engine* engine;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	void createDescriptorSetLayout();
};

#include "Engine.h"