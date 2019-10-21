#pragma once

#include <vulkan/vulkan.h>

class Engine;

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice device);
void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, Engine* engine);
void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, Engine* engine);
void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, Engine* engine);
void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, Engine* engine);
void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, Engine* engine);
VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, Engine* engine);

#include "Engine.h"