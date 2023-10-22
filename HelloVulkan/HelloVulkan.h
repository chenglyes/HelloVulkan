#pragma once

#include <vector>

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
	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();

	bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
	std::vector<const char*> getRequiredExtensions();
	void createInstance();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugMessenger();
	bool isDeviceSuitable(VkPhysicalDevice device);
	void pickPhysicalDevice();
	uint32_t findQueueFamilies(VkPhysicalDevice device);

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

	VkInstance mInstance{};
	VkDebugUtilsMessengerEXT mDebugMessenger{};

};
