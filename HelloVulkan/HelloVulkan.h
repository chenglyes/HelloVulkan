#pragma once

#include <string>
#include <array>
#include <vector>
#include <optional>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>


class HelloVulkanApp
{
public:
	static constexpr uint32_t WIDTH = 800;
	static constexpr uint32_t HEIGHT = 600;

	static constexpr const char* MODEL_PATH = "Assets/Models/viking_room.obj";
	static constexpr const char* TEXTURE_PATH = "Assets/Textures/viking_room.png";

#ifdef NDEBUG
	static constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
	static constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif // NDEBUG

	static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

public:
	void run();

public:
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

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;

		bool operator==(const Vertex& other) const
		{
			return pos == other.pos && color == other.color && texCoord == other.texCoord;
		}

		static VkVertexInputBindingDescription getBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
		{
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

			return attributeDescriptions;
		}
	};

	struct UniformBufferObject
	{
		glm::vec2 foo;
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
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
	VkSampleCountFlagBits getMaxUsableSampleCount();
	void pickPhysicalDevice();
	void createLogicalDevice();
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void createSwapChain();
	void cleanupSwapChain();
	void recreateSwapChain();
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	void createImageViews();
	VkShaderModule createShaderModule(const std::vector<char>& code);
	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createColorResources();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
		VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();
	bool hasStencilComponent(VkFormat format);
	void createDepthResources();
	void createFrameBuffers();
	void createCommandPool();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void generateMipmaps(VkImage image, VkFormat format, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	void createTextureImage();
	void createTextureImageView();
	void createTextureSampler();
	void loadModel();
	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();
	void createCommandBuffers();
	void createSyncObjects();

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void updateUniformBuffer(uint32_t currentImage);
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
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

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
	VkDescriptorSetLayout mDescriptorSetLayout{};
	VkPipelineLayout mPipelineLayout{};
	VkPipeline mGraphicsPipeline{};
	std::vector<VkFramebuffer> mSwapChainFrameBuffers;
	VkCommandPool mCommandPool{};

	uint32_t mCurrentFrame{ 0 };
	std::vector<VkCommandBuffer> mCommandBuffers;
	std::vector<VkSemaphore> mImageAvailableSemaphores;
	std::vector<VkSemaphore> mRenderFinishedSemaphores;
	std::vector<VkFence> mInFlightFences;

	bool mFrameBufferResized{ false };

	std::vector<Vertex> mVertices;
	std::vector<uint32_t> mIndices;

	VkBuffer mVertexBuffer{};
	VkDeviceMemory mVertexBufferMemory{};
	VkBuffer mIndexBuffer{};
	VkDeviceMemory mIndexBufferMemory{};

	std::vector<VkBuffer> mUniformBuffers;
	std::vector<VkDeviceMemory> mUniformBuffersMemory;
	std::vector<void*> mUniformBuffersMapped;

	VkDescriptorPool mDescriptorPool{};
	std::vector<VkDescriptorSet> mDescriptorSets{};

	uint32_t mMipLevels{ 0 };
	VkImage mTextureImage{};
	VkDeviceMemory mTextureImageMemory{};
	VkImageView mTextureImageView{};
	VkSampler mTextureSampler{};

	VkImage mDepthImage{};
	VkDeviceMemory mDepthImageMemory{};
	VkImageView mDepthImageView{};

	VkSampleCountFlagBits mMSAASampless{ VK_SAMPLE_COUNT_1_BIT };
	VkImage mColorImage{};
	VkDeviceMemory mColorImageMemory{};
	VkImageView mColorImageView{};

};
