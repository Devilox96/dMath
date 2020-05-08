#ifndef RENDERER_H
#define RENDERER_H
//-----------------------------//
#include <vector>
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <array>
#include <set>
#include <sstream>
//-----------------------------//
#include <vulkan/vulkan.h>
//-----------------------------//
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
//-----------------------------//
struct Vertex {
    float pos[2];
    float color[3];

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription BindingDescription = {};

        BindingDescription.binding = 0;
        BindingDescription.stride = 5 * sizeof(float);
        BindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return BindingDescription;
    }

    static std::array <VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array <VkVertexInputAttributeDescription, 2> AttributeDescriptions = {};

        AttributeDescriptions[0].binding = 0;
        AttributeDescriptions[0].location = 0;
        AttributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        AttributeDescriptions[0].offset = offsetof(Vertex, pos);

        AttributeDescriptions[1].binding = 0;
        AttributeDescriptions[1].location = 1;
        AttributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        AttributeDescriptions[1].offset = offsetof(Vertex, color);

        return AttributeDescriptions;
    }
};


//const std::vector <uint16_t> Indices = {
//        0, 1, 2, 2, 1, 3,
//        2, 3, 4, 4, 3, 5,
//        4, 5, 6, 6, 5, 7,
//        6, 7, 8, 8, 7, 9,
//        8, 9, 10, 10, 9, 11,
//        10, 11, 12, 12, 11, 13
//};
//-----------------------------//
class Renderer {
public:
    explicit Renderer(SDL_Window* tWindow, bool tDebug = false);
    ~Renderer();

    void drawFrame(const std::vector <std::vector <double>>& tData);
    VkDevice getDevice() {
        return mDevice;
    }
    void setResized() {
        mFrameBufferResized = true;
    }

private:
    enum VulkanErrorType {
        DEBUG_UTILS_EXTENSION_ERROR,
        DEBUG_UTILS_MESSENGER_ERROR,
        INSTANCE_CREATION_ERROR,
        LOGICAL_DEVICE_CREATION_ERROR,
        SURFACE_CREATION_ERROR,
        SWAP_CHAIN_CREATION_ERROR,
        IMAGE_VIEW_CREATION_ERROR,
        NO_VULKAN_GPU_ERROR,
        NO_GRAPHICS_QUEUE_FAMILY,
        NO_PRESENT_QUEUE_FAMILY,
        NO_SURFACE_FORMATS,
        NO_PRESENT_MODES
    };

    static std::string getError(VulkanErrorType tErrorType);

    bool mDebug = false;

    VkInstance mInstance                            = VK_NULL_HANDLE;
    VkPhysicalDevice mGPU                           = VK_NULL_HANDLE;
    VkDevice mDevice                                = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties mPropertiesGPU       = {};
    VkSurfaceKHR mSurface                           = VK_NULL_HANDLE;
    VkSurfaceCapabilitiesKHR mSurfaceCapabilities   = {};
    VkSwapchainKHR mSwapChain                       = VK_NULL_HANDLE;
    VkRenderPass mRenderPass                        = VK_NULL_HANDLE;
    VkPipelineLayout mLayout                        = VK_NULL_HANDLE;
    VkPipeline mPipeline                            = VK_NULL_HANDLE;
    uint32_t mSwapChainImageCount                   = 0;

    uint32_t mGraphicsFamilyIndex                   = 0;
    uint32_t mPresentFamilyIndex                    = 0;

    VkQueue mGraphicsQueue                          = VK_NULL_HANDLE;
    VkQueue mPresentQueue                           = VK_NULL_HANDLE;

    VkSurfaceFormatKHR mSurfaceFormat               = {};
    VkPresentModeKHR mPresentMode;

    VkExtent2D mSwapchainExtent                     = {};

    VkCommandPool mCommandPool                      = VK_NULL_HANDLE;

    VkBuffer mVertexBuffer;
    VkDeviceMemory mVertexBufferMemory;
    VkBuffer mIndexBuffer;
    VkDeviceMemory mIndexBufferMemory;

    std::vector <VkSemaphore>  mImageAvailableSemaphores;
    std::vector <VkSemaphore>  mRenderFinishedSemaphores;
    std::vector <VkFence> mFences;
    std::vector <VkFence> mImagesInFlight;

    std::vector <const char*> mLayersInstance;
    std::vector <const char*> mExtensionsInstance;

    std::vector <const char*> mExtensionsDevice;

    std::vector <VkImage> mSwapchainImages;
    std::vector <VkImageView> mSwapchainImageViews;
    std::vector <VkFramebuffer> mSwapchainFramebuffers;
    std::vector <VkCommandBuffer> mCommandBuffers;

    SDL_Window* mWindow;

    const int MAX_FRAMES_IN_FLIGHT = 2;

    size_t mCurrentFrame = 0;
    bool mFrameBufferResized = false;

    std::vector <Vertex> Vertices;
    std::vector <uint16_t> Indices;

    double mDarkBlue = -1.0e-09;
    double mBlue;
    double mCyan;
    double mGreen;
    double mYellow;
    double mRed;
    double mDarkRed = 1.0e-09;

    std::vector <std::vector <double>> mData;

    //----------//

    static std::vector <char> readFile(const std::string& tFileName);
    VkShaderModule createShaderModule(const std::vector <char>& tCode);
    void reinitSwapchain();
    uint32_t findMemoryType(uint32_t tTypeFilter, VkMemoryPropertyFlags tProperties);
    void createBuffer(
            VkDeviceSize tSize,
            VkBufferUsageFlags tUsage,
            VkMemoryPropertyFlags tProperties,
            VkBuffer& tBuffer,
            VkDeviceMemory& tBufferMemory);
    void copyBuffer(VkBuffer tSrcBuffer, VkBuffer tDstBuffer, VkDeviceSize  tSize);
    void loadPlot(const std::string& tPath);
    void fillGrid(uint16_t tSizeX, uint16_t tSizeY);

    void setLayers();
    void setExtensions();

    void initInstance();
    void initSurface();
    void initGPU();
    void setQueueFamilyIndices();
    void createLogicalDevice();
    void setFormat();
    void setPresentMode();
    void setSwapExtent();
    void createSwapchain();
    void initSwapchainImages();
    void initRenderPass();
    void initGraphicsPipeline();
    void initFrameBuffers();
    void initCommandPool();
    void initVertexBuffer();
    void initIndexBuffer();
    void initCommandBuffers();
    void initSyncObjects();

    //----------//

    VkDebugUtilsMessengerEXT mDebugReport           = VK_NULL_HANDLE;
    VkDebugUtilsMessengerCreateInfoEXT mDebugInfo   = {};

    //----------//

    void initDebug();
    void destroyDebug();
};
//-----------------------------//
#endif

