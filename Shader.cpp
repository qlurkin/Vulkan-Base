#include "Shader.h"
#include "Log.h"

#include <vector>
#include <fstream>

static std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

Shader::Shader(const std::string& filename, Engine* engine) : filename(filename), engine(engine) {
	auto code = readFile(filename);

	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	if (vkCreateShaderModule(engine->getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	LOG << "Shader (" << filename << ") Created" << ENDL;
}

Shader::~Shader() {
	vkDestroyShaderModule(engine->getDevice(), shaderModule, nullptr);
	LOG << "Shader (" << filename << ") Destroyed" << ENDL;
}

Shader::operator VkShaderModule() {
	return shaderModule;
}