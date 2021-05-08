//
// Created by devilox on 10/14/20.
//
//-----------------------------//
#ifndef DPLOT_RENDERER_H
#define DPLOT_RENDERER_H
//-----------------------------//
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <set>
#include <algorithm>
#include <fstream>
#include <array>
//-----------------------------//
#include <vulkan/vulkan.h>
#include <SDL2/SDL_vulkan.h>
//-----------------------------//
#include "dVulkanMesh.h"
#include "dDensityPlot2D.h"
//-----------------------------//
class Renderer {
public:
    explicit Renderer(SDL_Window* tWindow);
    ~Renderer();

    //----------//

    void setApplicationName(const std::string& tAppName);
    void setApplicationVersion(uint32_t tMajor, uint32_t tMinor, uint32_t tPatch);
    void setDebugMode(bool tState);

    void init();
    void draw(const std::vector <std::vector <float>>& tNewData);
private:
    std::string                     mAppName;
    uint32_t                        mAppVersion                     = 0;

    bool                            mDebug                          = false;

    //----------//

    SDL_Window*                     mWindow                         = nullptr;
    std::vector <dVulkanMesh>       mMeshList;
    size_t                          mCurrentFrame                   = 0;

    dDensityPlot2D <360, 50>*       mTestPlot                       = nullptr;

    std::vector <VkBuffer*>         mPlotBuffers;

    //----------//

    std::vector <const char*>       mInstanceLayers;
    std::vector <const char*>       mInstanceExtensions;

    std::vector <const char*>       mDeviceExtensions;

    //----------//

    VkInstance                      mInstance                       = VK_NULL_HANDLE;
    VkSurfaceKHR                    mSurface                        = VK_NULL_HANDLE;
    VkPhysicalDevice                mGPU                            = VK_NULL_HANDLE;
    VkDevice                        mLogicalGPU                     = VK_NULL_HANDLE;
    VkQueue                         mGraphicsQueue                  = VK_NULL_HANDLE;
    VkQueue                         mPresentationQueue              = VK_NULL_HANDLE;
    int32_t                         mGraphicsFamilyIndex;
    int32_t                         mPresentationFamilyIndex;
    VkSwapchainKHR                  mSwapchain                      = VK_NULL_HANDLE;
    VkFormat                        mFormat;
    VkExtent2D                      mExtent;

    std::vector <VkImage>           mSwapchainImages;
    std::vector <VkImageView>       mSwapchainImageViews;
    std::vector <VkFramebuffer>     mSwapchainFrameBuffers;
    std::vector <VkCommandBuffer>   mCommandBuffers;

    size_t                          mMaxQueueImages                 = 2;

    VkCommandPool                   mGraphicsCommandPool;

    VkPipeline                      mGraphicsPipeline               = VK_NULL_HANDLE;
    VkPipelineLayout                mPipelineLayout                 = VK_NULL_HANDLE;

    VkRenderPass                    mRenderPass                     = VK_NULL_HANDLE;

    std::vector <VkSemaphore>       mImageAvailable;
    std::vector <VkSemaphore>       mRenderFinished;
    std::vector <VkFence>           mDrawFences;

    //----------//

    void createInstance();
    void createSurface();
    void setPhysicalDevice();
    void createLogicalDevice();
    void createSwapchain();
    void createRenderPass();
    void createPipeline();
    void createFrameBuffers();
    void createCommandPool();
    void createCommandBuffers();
    void createSynchronization();

    void recordCommands();

    //----------//

    void destroyAll();
    void setInstanceExtensions();
    bool checkDeviceExtensions(VkPhysicalDevice tDevice);
    VkShaderModule loadShader(const std::string& tPath);

    static int32_t getGraphicsQueueFamilyIndex(VkPhysicalDevice tDevice);
    static int32_t getPresentationQueueFamilyIndex(VkPhysicalDevice tDevice, VkSurfaceKHR tSurface);
};
//-----------------------------//
#endif
