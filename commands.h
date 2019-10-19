#pragma once

#include <vulkan/vulkan.h>

class Engine;

VkCommandBuffer beginSingleTimeCommands(Engine* engine);
void endSingleTimeCommands(VkCommandBuffer commandBuffer, Engine* engine);

#include "Engine.h"