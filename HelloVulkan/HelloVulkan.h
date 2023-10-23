#pragma once

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
	bool isDeviceSuitable(VkPhysicalDevice device);
	void pickPhysicalDevice();
	void createLogicalDevice();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
	static VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator);

private:
	GLFWwindow* mWindow{ nullptr };

	const std::vector<const char*> mValidationLayers{ "VK_LAYER_KHRONOS_validation" };

	VkInstance mInstance{};
	VkDebugUtilsMessengerEXT mDebugMessenger{};
	VkSurfaceKHR mSurface{};
	VkPhysicalDevice mPhysicalDevice{};
	VkDevice mDevice{};
	VkQueue mGraphicsQueue{};
	VkQueue mPresentQueue{};

};
