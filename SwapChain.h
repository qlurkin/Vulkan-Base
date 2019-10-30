#pragma once

#include <vulkan/vulkan.h>
#include <list>
#include <vector>

class Engine;
class SwapChain;
class CommandBuffer;

class SwapChainObserver {
	public:
	SwapChainObserver(SwapChain& swapChain);
	virtual ~SwapChainObserver();
	virtual void cleanup() = 0;
	virtual void creation(SwapChain& swapchain) = 0;
	SwapChain& getSwapchain();

	private:
	SwapChain& swapChain;
};

class SwapChain {
	public:
	SwapChain(int windowWidth, int windowHeight, Engine* engine);
	~SwapChain();
	void subscribe(SwapChainObserver* observer);
	void unsubscribe(SwapChainObserver* observer);
	uint32_t getWidth();
	uint32_t getHeight();
	uint32_t getCurrentImage();
	VkDescriptorPool getDescriptorPool();
	VkRenderPass getRenderPass();
	VkExtent2D getExtent();
	VkFormat getFormat();
	size_t getSwapChainBuffersCount();
	VkFramebuffer getSwapChainBuffer(size_t i);
	void recreate(int windowWidth, int windowHeight);
	void acquireNextImage();
	void submitCommands(std::vector<CommandBuffer*> CommandBuffers);
	void presentImage();
	void setFramebufferResized();

	private:
	void createSwapChain(int windowWidth, int windowHeight);
	void cleanupSwapChain();
	void NotifyCleanup();
	void NotifyCreation();
	void createDepthResources();
	void createDescriptorPool();
	void createImageViews();
	void createFramebuffers();
	void createRenderPass();
	void createSyncObjects();
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, int windowWidth, int windowHeight);

	Engine* engine;
	std::list<SwapChainObserver*> observers;
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	size_t currentFrame = 0;
	uint32_t imageIndex;
	bool framebufferResized = false;
	
	// TODO: Should this go to Pipeline?
	VkRenderPass renderPass;
	VkDescriptorPool descriptorPool;
};

#include "Engine.h"