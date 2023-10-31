#pragma once

#include <string>
#include <vector>
#include <optional>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>


class HelloVulkanApp
{
public:
	static constexpr uint32_t WIDTH = 800;
	static constexpr uint32_t HEIGHT = 600;

#ifdef NDEBUG
	static constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
	static constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif // NDEBUG

public:
	void run();

private:
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

private:
	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();

	bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
	std::vector<const char*> getRequiredExtensions();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void createInstance();
	void setupDebugMessenger();
	void createSurface();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	void pickPhysicalDevice();
	void createLogicalDevice();
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void createSwapChain();
	void createImageViews();
	VkShaderModule createShaderModule(const std::vector<char>& code);
	void createRenderPass();
	void createGraphicsPipeline();
	void createFrameBuffers();
	void createCommandPool();
	void createCommandBuffer();
	void createSyncObjects();

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void drawFrame();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
	static VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator);
	static std::vector<char> readFile(const std::string& filename);

private:
	GLFWwindow* mWindow{ nullptr };

	const std::vector<const char*> mValidationLayers{ "VK_LAYER_KHRONOS_validation" };
	const std::vector<const char*> mDeviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	VkInstance mInstance{};
	VkDebugUtilsMessengerEXT mDebugMessenger{};
	VkSurfaceKHR mSurface{};
	VkPhysicalDevice mPhysicalDevice{};
	VkDevice mDevice{};
	VkQueue mGraphicsQueue{};
	VkQueue mPresentQueue{};
	VkSwapchainKHR mSwapChain{};
	std::vector<VkImage> mSwapChainImages;
	std::vector<VkImageView> mSwapChainImageViews;
	VkFormat mSwapChainImageFormat{};
	VkExtent2D mSwapChainExtent{};
	VkRenderPass mRenderPass{};
	VkPipelineLayout mPipelineLayout{};
	VkPipeline mGraphicsPipeline{};
	std::vector<VkFramebuffer> mSwapChainFrameBuffers;
	VkCommandPool mCommandPool{};
	VkCommandBuffer mCommandBuffer{};
	VkSemaphore mImageAvailableSemaphore{};
	VkSemaphore mRenderFinishedSemaphore{};
	VkFence mInFlightFence{};

};
