#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <optional>
#include <set>

#include "VertexInputState.h"
#include "Shader.h"
#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "CommandBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "Texture.h"
#include "Log.h"

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class Engine {
public:
	int run();
	virtual void initPipelines() = 0;
	virtual void setup() = 0;
	virtual void setupFrame() = 0;
	virtual void teardown() = 0;
	virtual void cleanupPipelines() = 0;
	void add(CommandBuffer* commandBuffer);
	VkPhysicalDevice getPhysicalDevice();
	VkDevice getDevice();
	VkExtent2D getExtent();
	VkFormat getFormat();
	size_t getSwapChainBuffersCount();
	VkFramebuffer getSwapChainBuffer(size_t i);
	VkCommandPool getCommandPool();
	VkRenderPass getRenderPass();
	VkQueue getGraphicsQueue();
	uint32_t getWidth();
	uint32_t getHeight();
	uint32_t getCurrentImage();
	VkDescriptorPool getDescriptorPool();

private:
	GLFWwindow* window;

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkRenderPass renderPass;
	
	//VkPipelineLayout pipelineLayout;
	//VkPipeline graphicsPipeline;

	VkCommandPool commandPool;
	VkDescriptorPool descriptorPool;

	std::vector<VkBuffer> vertexBuffers;
	std::vector<VkDeviceMemory> vertexBufferMemories;

	std::vector<CommandBuffer*> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	size_t currentFrame = 0;
	uint32_t imageIndex;

	std::string vertexShaderFilename;
	std::string fragmentShaderFilename;

	//VertexInputState& vertexInputState;

	bool framebufferResized = false;

	void initWindow();
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	void initVulkan();
	void mainLoop();
	void cleanupSwapChain();
	void cleanup();
	void recreateSwapChain();
	void createInstance();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugMessenger();
	void createSurface();
	void createRenderPass();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();
	void createImageViews();
	void createFramebuffers();
	void createCommandPool();
	void createDescriptorPool();
	void createDescriptorSets();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createSyncObjects();
	void drawFrame();
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
};