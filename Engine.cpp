#include "Engine.h"
#include "Log.h"
#include "buffers.h"

const int WIDTH = 1024;
const int HEIGHT = 768;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

// VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, Engine* engine) {
// 	for (VkFormat format : candidates) {
// 		VkFormatProperties props;
// 		vkGetPhysicalDeviceFormatProperties(engine->getPhysicalDevice(), format, &props);

// 		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
// 			return format;
// 		} else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
// 			return format;
// 		}
// 	}

// 	throw std::runtime_error("failed to find supported format!");
// }

// VkFormat findDepthFormat(Engine* engine) {
// 	return findSupportedFormat(
// 		{VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
// 		VK_IMAGE_TILING_OPTIMAL,
// 		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
// 		engine
// 	);
// }

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

int Engine::run() {
	try {
		initWindow();
		initVulkan();
		setup();
		//initPipelines();
		mainLoop();
		teardown();
		cleanup();
	} catch (const std::exception& e) {
		LOG << e.what() << ENDL;
		return EXIT_FAILURE;
	}

	Log::destroy();
	return EXIT_SUCCESS;
}

void Engine::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	//glfwSetWindowUserPointer(window, this);
	//glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void Engine::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
	//app->framebufferResized = true;
	app->swapChain->setFramebufferResized();
}

void Engine::initVulkan() {
	createInstance();
	setupDebugMessenger();
	createSurface();
	pickPhysicalDevice();
	createLogicalDevice();
	createCommandPool();
	swapChain = new SwapChain(WIDTH, HEIGHT, this);
	//createSwapChain();
	//createImageViews();
	//createRenderPass();
	
	//createDepthResources();
	//createFramebuffers();
	//createSyncObjects();
	//createDescriptorPool();
}

void Engine::mainLoop() {
	LOG << "Mainloop Started" << ENDL;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		drawFrame();
	}

	vkDeviceWaitIdle(device);
	LOG << "Mainloop Terminated" << ENDL;
}

// void Engine::cleanupSwapChain() {
// 	vkDestroyImageView(device, depthImageView, nullptr);
// 	vkDestroyImage(device, depthImage, nullptr);
// 	vkFreeMemory(device, depthImageMemory, nullptr);

// 	for (auto framebuffer : swapChainFramebuffers) {
// 		vkDestroyFramebuffer(device, framebuffer, nullptr);
// 	}

// 	cleanupPipelines();
// 	vkDestroyRenderPass(device, renderPass, nullptr);
	

// 	for (auto imageView : swapChainImageViews) {
// 		vkDestroyImageView(device, imageView, nullptr);
// 	}

// 	vkDestroySwapchainKHR(device, swapChain, nullptr);
// 	vkDestroyDescriptorPool(device, descriptorPool, nullptr);
// }

void Engine::cleanup() {
	//cleanupSwapChain();
	delete swapChain;

	for(auto vertexBuffer: vertexBuffers) {
		vkDestroyBuffer(device, vertexBuffer, nullptr);
	}

	for(auto vertexBufferMemory: vertexBufferMemories) {
		vkFreeMemory(device, vertexBufferMemory, nullptr);
	}

	// for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
	// 	vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
	// 	vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
	// 	vkDestroyFence(device, inFlightFences[i], nullptr);
	// }

	vkDestroyCommandPool(device, commandPool, nullptr);

	vkDestroyDevice(device, nullptr);

	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);

	glfwTerminate();
}

// void Engine::recreateSwapChain() {
// 	int width = 0, height = 0;
// 	while (width == 0 || height == 0) {
// 		glfwGetFramebufferSize(window, &width, &height);
// 		glfwWaitEvents();
// 	}

// 	vkDeviceWaitIdle(device);

// 	cleanupSwapChain();

// 	createSwapChain();
// 	createImageViews();
// 	createRenderPass();
// 	createDepthResources();
// 	createFramebuffers();
// 	createDescriptorPool();
// 	initPipelines();
// }

void Engine::createInstance() {
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
	} else {
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
}

void Engine::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}

void Engine::setupDebugMessenger() {
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug messenger!");
	}
}

void Engine::createSurface() {
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}

void Engine::pickPhysicalDevice() {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (isDeviceSuitable(device)) {
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

void Engine::createLogicalDevice() {
	queueFamilyIndices = findQueueFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = {queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value()};

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	} else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(device, queueFamilyIndices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, queueFamilyIndices.presentFamily.value(), 0, &presentQueue);
}

// void Engine::createSwapChain() {
// 	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

// 	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
// 	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
// 	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

// 	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
// 	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
// 		imageCount = swapChainSupport.capabilities.maxImageCount;
// 	}

// 	VkSwapchainCreateInfoKHR createInfo = {};
// 	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
// 	createInfo.surface = surface;

// 	createInfo.minImageCount = imageCount;
// 	createInfo.imageFormat = surfaceFormat.format;
// 	createInfo.imageColorSpace = surfaceFormat.colorSpace;
// 	createInfo.imageExtent = extent;
// 	createInfo.imageArrayLayers = 1;
// 	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

// 	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
// 	uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

// 	if (indices.graphicsFamily != indices.presentFamily) {
// 		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
// 		createInfo.queueFamilyIndexCount = 2;
// 		createInfo.pQueueFamilyIndices = queueFamilyIndices;
// 	} else {
// 		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
// 	}

// 	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
// 	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
// 	createInfo.presentMode = presentMode;
// 	createInfo.clipped = VK_TRUE;

// 	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
// 		throw std::runtime_error("failed to create swap chain!");
// 	}

// 	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
// 	swapChainImages.resize(imageCount);
// 	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

// 	swapChainImageFormat = surfaceFormat.format;
// 	swapChainExtent = extent;
// }

// void Engine::createImageViews() {
// 	swapChainImageViews.resize(swapChainImages.size());
	
// 	for (uint32_t i = 0; i < swapChainImages.size(); i++) {
// 		swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, this);
// 	}
// }

// void Engine::createRenderPass() {
// 	VkAttachmentDescription colorAttachment = {};
// 	colorAttachment.format = swapChainImageFormat;
// 	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
// 	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
// 	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
// 	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
// 	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
// 	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
// 	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

// 	VkAttachmentDescription depthAttachment = {};
// 	depthAttachment.format = findDepthFormat(this);
// 	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
// 	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
// 	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
// 	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
// 	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
// 	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
// 	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

// 	VkAttachmentReference colorAttachmentRef = {};
// 	colorAttachmentRef.attachment = 0;
// 	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

// 	VkAttachmentReference depthAttachmentRef = {};
// 	depthAttachmentRef.attachment = 1;
// 	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

// 	VkSubpassDescription subpass = {};
// 	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
// 	subpass.colorAttachmentCount = 1;
// 	subpass.pColorAttachments = &colorAttachmentRef;
// 	subpass.pDepthStencilAttachment = &depthAttachmentRef;

// 	VkSubpassDependency dependency = {};
// 	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
// 	dependency.dstSubpass = 0;
// 	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
// 	dependency.srcAccessMask = 0;
// 	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
// 	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

// 	std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
// 	VkRenderPassCreateInfo renderPassInfo = {};
// 	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
// 	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());;
// 	renderPassInfo.pAttachments = attachments.data();
// 	renderPassInfo.subpassCount = 1;
// 	renderPassInfo.pSubpasses = &subpass;
// 	renderPassInfo.dependencyCount = 1;
// 	renderPassInfo.pDependencies = &dependency;

// 	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
// 		throw std::runtime_error("failed to create render pass!");
// 	}
// }

// void Engine::createFramebuffers() {
// 	swapChainFramebuffers.resize(swapChainImageViews.size());

// 	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
// 		std::array<VkImageView, 2> attachments = {
// 			swapChainImageViews[i],
// 			depthImageView
// 		};

// 		VkFramebufferCreateInfo framebufferInfo = {};
// 		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
// 		framebufferInfo.renderPass = renderPass;
// 		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
// 		framebufferInfo.pAttachments = attachments.data();
// 		framebufferInfo.width = swapChainExtent.width;
// 		framebufferInfo.height = swapChainExtent.height;
// 		framebufferInfo.layers = 1;

// 		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
// 			throw std::runtime_error("failed to create framebuffer!");
// 		}
// 	}
// }

void Engine::createCommandPool() {
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

uint32_t Engine::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

// void Engine::createSyncObjects() {
// 	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
// 	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
// 	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

// 	VkSemaphoreCreateInfo semaphoreInfo = {};
// 	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

// 	VkFenceCreateInfo fenceInfo = {};
// 	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
// 	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

// 	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
// 		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
// 			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
// 			vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
// 			throw std::runtime_error("failed to create synchronization objects for a frame!");
// 		}
// 	}
// }

void Engine::drawFrame() {
	// vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	// VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	// if (result == VK_ERROR_OUT_OF_DATE_KHR) {
	// 	recreateSwapChain();
	// 	return;
	// } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
	// 	throw std::runtime_error("failed to acquire swap chain image!");
	// }
	swapChain->acquireNextImage();

	setupFrame();

	// VkSubmitInfo submitInfo = {};
	// submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	// VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
	// VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	// submitInfo.waitSemaphoreCount = 1;
	// submitInfo.pWaitSemaphores = waitSemaphores;
	// submitInfo.pWaitDstStageMask = waitStages;

	// VkCommandBuffer* cmdBuffers = new VkCommandBuffer[commandBuffers.size()];
	// for(int i=0; i < commandBuffers.size(); i++) {
	// 	cmdBuffers[i] = (*(commandBuffers[i]))[imageIndex];
	// }


	// submitInfo.commandBufferCount = commandBuffers.size();
	// submitInfo.pCommandBuffers = cmdBuffers;

	// VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
	// submitInfo.signalSemaphoreCount = 1;
	// submitInfo.pSignalSemaphores = signalSemaphores;

	// vkResetFences(device, 1, &inFlightFences[currentFrame]);

	// if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
	// 	throw std::runtime_error("failed to submit draw command buffer!");
	// }

	// delete[] cmdBuffers;
	swapChain->submitCommands(commandBuffers);
	
	commandBuffers.clear();

	// VkPresentInfoKHR presentInfo = {};
	// presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	// presentInfo.waitSemaphoreCount = 1;
	// presentInfo.pWaitSemaphores = signalSemaphores;

	// VkSwapchainKHR swapChains[] = {swapChain};
	// presentInfo.swapchainCount = 1;
	// presentInfo.pSwapchains = swapChains;

	// presentInfo.pImageIndices = &imageIndex;

	// result = vkQueuePresentKHR(presentQueue, &presentInfo);

	// if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
	// 	framebufferResized = false;
	// 	recreateSwapChain();
	// } else if (result != VK_SUCCESS) {
	// 	throw std::runtime_error("failed to present swap chain image!");
	// }

	// currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	swapChain->presentImage();
}

// VkSurfaceFormatKHR Engine::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
// 	for (const auto& availableFormat : availableFormats) {
// 		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
// 			return availableFormat;
// 		}
// 	}

// 	return availableFormats[0];
// }

// VkPresentModeKHR Engine::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
// 	for (const auto& availablePresentMode : availablePresentModes) {
// 		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
// 			return availablePresentMode;
// 		}
// 	}

// 	return VK_PRESENT_MODE_FIFO_KHR;
// }

// VkExtent2D Engine::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
// 	if (capabilities.currentExtent.width != UINT32_MAX) {
// 		return capabilities.currentExtent;
// 	} else {
// 		int width, height;
// 		glfwGetFramebufferSize(window, &width, &height);

// 		VkExtent2D actualExtent = {
// 			static_cast<uint32_t>(width),
// 			static_cast<uint32_t>(height)
// 		};

// 		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
// 		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

// 		return actualExtent;
// 	}
// }

SwapChainSupportDetails Engine::querySwapChainSupport(VkPhysicalDevice device) {
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

bool Engine::isDeviceSuitable(VkPhysicalDevice device) {
	QueueFamilyIndices indices = findQueueFamilies(device);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

bool Engine::checkDeviceExtensionSupport(VkPhysicalDevice device) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

QueueFamilyIndices Engine::findQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (queueFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

std::vector<const char*> Engine::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool Engine::checkValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Engine::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	LOG << "validation layer: " << pCallbackData->pMessage << ENDL;

	return VK_FALSE;
}

VkPhysicalDevice Engine::getPhysicalDevice() {
	return physicalDevice;
}

VkDevice Engine::getDevice() {
	return device;
}

// VkExtent2D Engine::getExtent() {
// 	return swapChainExtent;
// }

// VkFormat Engine::getFormat() {
// 	return swapChainImageFormat;
// }

// size_t Engine::getSwapChainBuffersCount() {
// 	return swapChainFramebuffers.size();
// }

// VkFramebuffer Engine::getSwapChainBuffer(size_t i) {
// 	return swapChainFramebuffers[i];
// }

VkCommandPool Engine::getCommandPool() {
	return commandPool;
}

// VkRenderPass Engine::getRenderPass() {
// 	return renderPass;
// }

VkQueue Engine::getGraphicsQueue() {
	return graphicsQueue;
}

void Engine::add(CommandBuffer& commandBuffer) {
	commandBuffers.push_back(&commandBuffer);
}

uint32_t Engine::getWidth() {
	return swapChain->getWidth();
}

uint32_t Engine::getHeight() {
	return swapChain->getHeight();
}

// uint32_t Engine::getCurrentImage() {
// 	return imageIndex;
// }

// void Engine::createDescriptorPool() {
// 	std::array<VkDescriptorPoolSize, 2> poolSizes = {};
// 	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
// 	poolSizes[0].descriptorCount = static_cast<uint32_t>(swapChainImages.size());
// 	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
// 	poolSizes[1].descriptorCount = static_cast<uint32_t>(swapChainImages.size());

// 	VkDescriptorPoolCreateInfo poolInfo = {};
// 	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
// 	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
// 	poolInfo.pPoolSizes = poolSizes.data();
// 	poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());

// 	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
// 		throw std::runtime_error("failed to create descriptor pool!");
// 	}
// }

// VkDescriptorPool Engine::getDescriptorPool() {
// 	return descriptorPool;
// }

// void Engine::createDepthResources() {
// 	VkFormat depthFormat = findDepthFormat(this);

// 	createImage(swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory, this);
// 	depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, this);

// 	transitionImageLayout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, this);
// }

VkSurfaceKHR Engine::getSurface() {
	return surface;
}

QueueFamilyIndices Engine::getQueueFamilyIndices() {
	return queueFamilyIndices;
}

void Engine::getWindowSize(int& width, int& height) {
	width = 0;
	height = 0;
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}
}

VkQueue Engine::getPresentQueue() {
	return presentQueue;
}

SwapChain& Engine::getSwapChain() {
	return *swapChain;
}