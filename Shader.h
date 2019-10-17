#pragma once

#include <vulkan/vulkan.h>
#include <string>

class Engine;

class Shader {
	public:
	Shader(const std::string& filename, Engine* engine);
	~Shader();
	operator VkShaderModule();

	private:
	Engine* engine;
	VkShaderModule shaderModule;
	const std::string filename;
};

#include "Engine.h"