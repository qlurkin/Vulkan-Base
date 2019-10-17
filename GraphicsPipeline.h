#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Engine;
class VertexInputState;
class Shader;
class CommandBuffer;
class VertexBuffer;
class CommandPool;

class GraphicsPipeline {
	public:
	GraphicsPipeline(VertexInputState vertexInputState, Shader& vertexShader, Shader& fragmentShader, Engine* engine);
	~GraphicsPipeline();
	operator VkPipeline();
	Engine* getEngine();

	private:
	Engine* engine;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	void createDescriptorSetLayout();
};

#include "Engine.h"