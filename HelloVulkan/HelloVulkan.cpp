#include "HelloVulkan.h"

#include <format>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cstdlib>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>


#define CHECK_AND_THROW(message, result) if (result != VK_SUCCESS) throw std::runtime_error(message);


void HelloVulkanApp::run()
{
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

void HelloVulkanApp::initWindow()
{
	::glfwInit();

	::glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	::glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	mWindow = ::glfwCreateWindow(WIDTH, HEIGHT, "Hello Vulkan", nullptr, nullptr);
}

void HelloVulkanApp::initVulkan()
{
	createInstance();
	setupDebugMessenger();
	pickPhysicalDevice();
}

void HelloVulkanApp::mainLoop()
{
	while (!::glfwWindowShouldClose(mWindow))
	{
		::glfwPollEvents();
	}
}

void HelloVulkanApp::cleanup()
{
	if (ENABLE_VALIDATION_LAYERS)
	{
		destroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
	}

	::vkDestroyInstance(mInstance, nullptr);

	::glfwDestroyWindow(mWindow);
	::glfwTerminate();
}

bool HelloVulkanApp::checkValidationLayerSupport(const std::vector<const char*>& validationLayers)
{
	uint32_t layerCount{ 0 };
	::vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	::vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers)
	{
		bool layerFound{ false };

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
			return false;
	}

	return true;
}

std::vector<const char*> HelloVulkanApp::getRequiredExtensions()
{
	uint32_t glfwExtensionCount{ 0 };
	const char** glfwExtensions = ::glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	if (ENABLE_VALIDATION_LAYERS)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	return extensions;
}

void HelloVulkanApp::createInstance()
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Vulkan";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	const std::vector<const char*> validationLayers{ "VK_LAYER_KHRONOS_validation" };

	if (ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport(validationLayers))
	{
		throw std::runtime_error("Validation layers requested, but not available!");
	}

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	if (ENABLE_VALIDATION_LAYERS)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateIndo{};
		populateDebugMessengerCreateInfo(debugCreateIndo);
		createInfo.pNext = &debugCreateIndo;
	}

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	CHECK_AND_THROW("Failed to create instance!", 
		::vkCreateInstance(&createInfo, nullptr, &mInstance));
}

void HelloVulkanApp::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
	createInfo.pfnUserCallback = debugCallBack;
	createInfo.pUserData = nullptr;
}

void HelloVulkanApp::setupDebugMessenger()
{
	if (!ENABLE_VALIDATION_LAYERS)
		return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	populateDebugMessengerCreateInfo(createInfo);

	CHECK_AND_THROW("Failed to setup debug messenger!",
		createDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr, &mDebugMessenger));
}

bool HelloVulkanApp::isDeviceSuitable(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties{};
	::vkGetPhysicalDeviceProperties(device, &deviceProperties);
	VkPhysicalDeviceFeatures deviceFeatures{};
	::vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	return true;
}

void HelloVulkanApp::pickPhysicalDevice()
{
	uint32_t deviceCount{ 0 };
	::vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr);

	if (deviceCount == 0)
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	::vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data());

	VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };
	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device))
		{
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE)
		throw std::runtime_error("Failed to find a suitable GPU!");
}

uint32_t HelloVulkanApp::findQueueFamilies(VkPhysicalDevice device)
{

}

VKAPI_ATTR VkBool32 VKAPI_CALL HelloVulkanApp::debugCallBack(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	// if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	std::cerr << std::format("Validation layer: {}", pCallbackData->pMessage) << std::endl;
	return VK_FALSE;
}

VkResult HelloVulkanApp::createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto pFunc = (PFN_vkCreateDebugUtilsMessengerEXT)::vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (pFunc != nullptr)
	{
		return pFunc(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}


void HelloVulkanApp::destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto pFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)::vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (pFunc != nullptr)
	{
		pFunc(instance, debugMessenger, pAllocator);
	}
}

int main()
{
	HelloVulkanApp app{};

	try
	{
		app.run();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
