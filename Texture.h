#pragma once

#include <vulkan/vulkan.h>
#include <string>

class Engine;

class Texture {
	public:
	Texture(const std::string& filename, Engine* engine);
	~Texture();

	private:
	Engine* engine;
	std::string filename;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	void createTextureImageView();
	void createTextureSampler();
};

#include "Engine.h"