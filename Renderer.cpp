#include "Renderer.h"
//-----------------------------//
Renderer::Renderer(SDL_Window* tWindow, bool tDebug) : mWindow(tWindow), mDebug(tDebug) {
    loadPlot("Amplitude2.dat");
    fillGrid(254, 50);

    setLayers();
    setExtensions();

    initInstance();
    initSurface();
    initGPU();
    setQueueFamilyIndices();
    createLogicalDevice();
    createSwapchain();
    initSwapchainImages();
    initRenderPass();
    initGraphicsPipeline();
    initFrameBuffers();
    initCommandPool();
    initVertexBuffer();
    initIndexBuffer();
    initCommandBuffers();
    initSyncObjects();
}
Renderer::~Renderer() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(mDevice, mRenderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(mDevice, mImageAvailableSemaphores[i], nullptr);
        vkDestroyFence(mDevice, mFences[i], nullptr);
    }

    vkDestroyCommandPool(mDevice, mCommandPool, nullptr);

    for (auto& iFrameBuffer : mSwapchainFramebuffers) {
        vkDestroyFramebuffer(mDevice, iFrameBuffer, nullptr);
    }

    vkDestroyPipeline(mDevice, mPipeline, nullptr);
    vkDestroyPipelineLayout(mDevice, mLayout, nullptr);
    vkDestroyRenderPass(mDevice, mRenderPass, nullptr);

    for (auto iView : mSwapchainImageViews) {
        vkDestroyImageView(mDevice, iView, nullptr);
    }

    vkDestroySwapchainKHR(mDevice, mSwapChain, nullptr);

    vkDestroyBuffer(mDevice, mIndexBuffer, nullptr);
    vkFreeMemory(mDevice, mIndexBufferMemory, nullptr);

    vkDestroyBuffer(mDevice, mVertexBuffer, nullptr);
    vkFreeMemory(mDevice, mVertexBufferMemory, nullptr);

    vkDestroyDevice(mDevice, nullptr);
    destroyDebug();
    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
    mWindow = nullptr;
    vkDestroyInstance(mInstance, nullptr);
}
//-----------------------------//
void Renderer::drawFrame(const std::vector <std::vector <double>>& tData) {
    vkWaitForFences(mDevice, 1, &mFences[mCurrentFrame], VK_TRUE, UINT64_MAX);

    uint16_t tSizeX = 254;
    uint16_t tSizeY = 50;
    
    for (uint16_t i = 0; i < tSizeX; i++) {
        for (uint16_t j = 0; j < tSizeY; j++) {
            if (tData[i][tSizeY - j - 1] <= mDarkBlue) {
                Vertices[j * tSizeX + i].color[0] = 0.0f;
                Vertices[j * tSizeX + i].color[1] = 0.0f;
                Vertices[j * tSizeX + i].color[2] = 0.2f;
            } else if (tData[i][tSizeY - j - 1] <= mBlue && tData[i][tSizeY - j - 1] > mDarkBlue) {
                Vertices[j * tSizeX + i].color[0] = 0.0f;
                Vertices[j * tSizeX + i].color[1] = 0.0f;
                Vertices[j * tSizeX + i].color[2] = 0.2f + float((tData[i][tSizeY - j - 1] - mDarkBlue) / (mBlue - mDarkBlue) * 0.8);
            } else if (tData[i][tSizeY - j - 1] <= mCyan && tData[i][tSizeY - j - 1] > mBlue) {
                Vertices[j * tSizeX + i].color[0] = 0.0f;
                Vertices[j * tSizeX + i].color[1] = float((tData[i][tSizeY - j - 1] - mBlue) / (mCyan - mBlue) * 1.0);
                Vertices[j * tSizeX + i].color[2] = 1.0f;
            } else if (tData[i][tSizeY - j - 1] <= mGreen && tData[i][tSizeY - j - 1] > mCyan) {
                Vertices[j * tSizeX + i].color[0] = 0.0f;
                Vertices[j * tSizeX + i].color[1] = 1.0f;
                Vertices[j * tSizeX + i].color[2] = 1.0f - float((tData[i][tSizeY - j - 1] - mCyan) / (mGreen - mCyan) * 1.0);
            } else if (tData[i][tSizeY - j - 1] <= mYellow && tData[i][tSizeY - j - 1] > mGreen) {
                Vertices[j * tSizeX + i].color[0] = float((tData[i][tSizeY - j - 1] - mGreen) / (mYellow - mGreen) * 1.0);
                Vertices[j * tSizeX + i].color[1] = 1.0f;
                Vertices[j * tSizeX + i].color[2] = 0.0f;
            } else if (tData[i][tSizeY - j - 1] <= mRed && tData[i][tSizeY - j - 1] > mYellow) {
                Vertices[j * tSizeX + i].color[0] = 1.0f;
                Vertices[j * tSizeX + i].color[1] = 1.0f - float((tData[i][tSizeY - j - 1] - mYellow) / (mRed - mYellow) * 1.0);
                Vertices[j * tSizeX + i].color[2] = 0.0f;
            } else if (tData[i][tSizeY - j - 1] <= mDarkRed && tData[i][tSizeY - j - 1] > mRed) {
                Vertices[j * tSizeX + i].color[0] = 1.0f - float((tData[i][tSizeY - j - 1] - mRed) / (mDarkRed - mRed) * 0.8);
                Vertices[j * tSizeX + i].color[1] = 0.0f;
                Vertices[j * tSizeX + i].color[2] = 0.0f;
            } else if (tData[i][tSizeY - j - 1] > mDarkRed) {
                Vertices[j * tSizeX + i].color[0] = 0.2f;
                Vertices[j * tSizeX + i].color[1] = 0.0f;
                Vertices[j * tSizeX + i].color[2] = 0.0f;
            }
        }
    }

    VkDeviceSize BufferSize = sizeof(float) * 5 * Vertices.size();

    void* Data;
    vkMapMemory(mDevice, mVertexBufferMemory, 0, BufferSize, 0, &Data);
    {
        memcpy(Data, Vertices.data(), size_t(BufferSize));
    }
    vkUnmapMemory(mDevice, mVertexBufferMemory);

    //----------//

    uint32_t ImageIndex;
    auto Result = vkAcquireNextImageKHR(mDevice, mSwapChain, UINT64_MAX, mImageAvailableSemaphores[mCurrentFrame], VK_NULL_HANDLE, &ImageIndex);

    if (Result == VK_ERROR_OUT_OF_DATE_KHR) {
        reinitSwapchain();
        return;
    } else if (Result != VK_SUCCESS && Result != VK_SUBOPTIMAL_KHR) {
        std::cout << "Failed to acquire swapchain image!" << std::endl;
        exit(-1);
    }

    if (mImagesInFlight[ImageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(mDevice, 1, &mImagesInFlight[ImageIndex], VK_TRUE, UINT64_MAX);
    }

    mImagesInFlight[ImageIndex] = mFences[mCurrentFrame];

    VkSemaphore WaitSemaphores[] = {mImageAvailableSemaphores[mCurrentFrame]};
    VkSemaphore SignalSemaphores[] = {mRenderFinishedSemaphores[mCurrentFrame]};
    VkPipelineStageFlags WaitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    VkSubmitInfo SubmitInfo = {};
    SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    SubmitInfo.waitSemaphoreCount = 1;
    SubmitInfo.pWaitSemaphores = WaitSemaphores;
    SubmitInfo.pWaitDstStageMask = WaitStages;
    SubmitInfo.commandBufferCount = 1;
    SubmitInfo.pCommandBuffers = &mCommandBuffers[ImageIndex];
    SubmitInfo.signalSemaphoreCount = 1;
    SubmitInfo.pSignalSemaphores = SignalSemaphores;

    vkResetFences(mDevice, 1, &mFences[mCurrentFrame]);

    if (vkQueueSubmit(mGraphicsQueue, 1, & SubmitInfo, mFences[mCurrentFrame]) != VK_SUCCESS) {
        std::cout << "Failed to submit draw command buffer!" << std::endl;
        exit(-1);
    }

    VkSwapchainKHR Swapchains[] = {mSwapChain};
    VkPresentInfoKHR PresentInfo = {};

    PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    PresentInfo.waitSemaphoreCount = 1;
    PresentInfo.pWaitSemaphores = SignalSemaphores;
    PresentInfo.swapchainCount = 1;
    PresentInfo.pSwapchains = Swapchains;
    PresentInfo.pImageIndices = &ImageIndex;
    PresentInfo.pResults = nullptr;

    Result = vkQueuePresentKHR(mGraphicsQueue, &PresentInfo);

    if (Result == VK_ERROR_OUT_OF_DATE_KHR || Result == VK_SUBOPTIMAL_KHR || mFrameBufferResized) {
        if (mFrameBufferResized) {
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
                    mGPU,
                    mSurface,
                    &mSurfaceCapabilities
            );
        }

        mFrameBufferResized = false;
        reinitSwapchain();
    } else if (Result != VK_SUCCESS) {
        std::cout << "Failed to present swapchain image!" << std::endl;
        exit(-1);
    }

    mCurrentFrame = (mCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
//-----------------------------//
std::vector <char> Renderer::readFile(const std::string& tFileName) {
    std::ifstream File(tFileName, std::ios::ate | std::ios::binary);

    if (!File.is_open()) {
        std::cout << "Failed to open the file!" << std::endl;
        exit(-1);
    }

    size_t FileSize = (size_t)File.tellg();
    std::vector <char> Buffer(FileSize);

    File.seekg(0);
    File.read(Buffer.data(), FileSize);
    File.close();

    return Buffer;
}
VkShaderModule Renderer::createShaderModule(const std::vector <char>& tCode) {
    VkShaderModuleCreateInfo Info = {};

    Info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    Info.codeSize = tCode.size();
    Info.pCode = reinterpret_cast <const uint32_t*>(tCode.data());

    VkShaderModule ShaderModule;

    if (vkCreateShaderModule(mDevice, &Info, nullptr, &ShaderModule) != VK_SUCCESS) {
        std::cout << "Failed to create the shader module!" << std::endl;
        exit(-1);
    }

    return ShaderModule;
}
void Renderer::reinitSwapchain() {
    vkDeviceWaitIdle(mDevice);

    for (auto& iFrameBuffer : mSwapchainFramebuffers) {
        vkDestroyFramebuffer(mDevice, iFrameBuffer, nullptr);
    }

    vkFreeCommandBuffers(
            mDevice,
            mCommandPool,
            static_cast <uint32_t>(mCommandBuffers.size()),
            mCommandBuffers.data());

    vkDestroyPipeline(mDevice, mPipeline, nullptr);
    vkDestroyPipelineLayout(mDevice, mLayout, nullptr);
    vkDestroyRenderPass(mDevice, mRenderPass, nullptr);

    for (auto iView : mSwapchainImageViews) {
        vkDestroyImageView(mDevice, iView, nullptr);
    }

    vkDestroySwapchainKHR(mDevice, mSwapChain, nullptr);

    createSwapchain();
    initSwapchainImages();
    initRenderPass();
    initGraphicsPipeline();
    initFrameBuffers();
    initCommandBuffers();
}
uint32_t Renderer::findMemoryType(uint32_t tTypeFilter, VkMemoryPropertyFlags tProperties) {
    VkPhysicalDeviceMemoryProperties MemProperties;
    vkGetPhysicalDeviceMemoryProperties(mGPU, &MemProperties);

    for (uint32_t i = 0; i < MemProperties.memoryTypeCount; i++) {
        if ((tTypeFilter & (1 << i)) && (MemProperties.memoryTypes[i].propertyFlags & tProperties) == tProperties) {
            return i;
        }
    }

    std::cout << "Failed to find suitable memory type!" << std::endl;
    exit(-1);
}
void Renderer::createBuffer(
        VkDeviceSize tSize,
        VkBufferUsageFlags tUsage,
        VkMemoryPropertyFlags tProperties,
        VkBuffer& tBuffer,
        VkDeviceMemory& tBufferMemory) {
    VkBufferCreateInfo BufferInfo = {};

    BufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    BufferInfo.size = tSize;
    BufferInfo.usage = tUsage;
    BufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(mDevice, &BufferInfo, nullptr, &tBuffer) != VK_SUCCESS) {
        std::cout << "Failed to create vertex buffer!" << std::endl;
        exit(-1);
    }

    VkMemoryRequirements MemRequirements;
    vkGetBufferMemoryRequirements(mDevice, tBuffer, &MemRequirements);

    VkMemoryAllocateInfo AllocInfo = {};

    AllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    AllocInfo.allocationSize = MemRequirements.size;
    AllocInfo.memoryTypeIndex = findMemoryType(MemRequirements.memoryTypeBits, tProperties);

    if (vkAllocateMemory(mDevice, &AllocInfo, nullptr, &tBufferMemory) != VK_SUCCESS) {
        std::cout << "Failed to allocate vertex buffer memory!" << std::endl;
        exit(-1);
    }

    vkBindBufferMemory(mDevice, tBuffer, tBufferMemory, 0);
}
void Renderer::copyBuffer(VkBuffer tSrcBuffer, VkBuffer tDstBuffer, VkDeviceSize  tSize) {
    VkCommandBufferAllocateInfo AllocInfo = {};

    AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    AllocInfo.commandPool = mCommandPool;
    AllocInfo.commandBufferCount = 1;

    VkCommandBuffer CommandBuffer;
    vkAllocateCommandBuffers(mDevice, &AllocInfo, &CommandBuffer);

    VkCommandBufferBeginInfo BeginInfo = {};

    BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(CommandBuffer, &BeginInfo);
    {
        VkBufferCopy CopyRegion = {};

        CopyRegion.srcOffset = 0;
        CopyRegion.dstOffset = 0;
        CopyRegion.size = tSize;

        vkCmdCopyBuffer(CommandBuffer, tSrcBuffer, tDstBuffer, 1, &CopyRegion);
    }
    vkEndCommandBuffer(CommandBuffer);

    VkSubmitInfo SubmitInfo = {};

    SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    SubmitInfo.commandBufferCount = 1;
    SubmitInfo.pCommandBuffers = &CommandBuffer;

    vkQueueSubmit(mGraphicsQueue, 1, &SubmitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(mGraphicsQueue);

    vkFreeCommandBuffers(mDevice, mCommandPool, 1, &CommandBuffer);
}
std::string Renderer::getError(VulkanErrorType tErrorType) {
    switch (tErrorType) {
        case DEBUG_UTILS_EXTENSION_ERROR:
            return "Error: debug extensions were not found!";
        case DEBUG_UTILS_MESSENGER_ERROR:
            return "Error: unable to set up a debug messenger!";
        case INSTANCE_CREATION_ERROR:
            return "Error: unable to create an instance!";
        case SURFACE_CREATION_ERROR:
            return "Error: unable to create a surface!";
        case LOGICAL_DEVICE_CREATION_ERROR:
            return "Error: unable to create a logical device!";
        case SWAP_CHAIN_CREATION_ERROR:
            return "Unable to create a swap chain!";
        case IMAGE_VIEW_CREATION_ERROR:
            return "Unable to create an image view!";
        case NO_VULKAN_GPU_ERROR:
            return "Error: unable to find GPUs with Vulkan support!";
        case NO_GRAPHICS_QUEUE_FAMILY:
            return "Error: unable to find the queue family with graphics bit!";
        case NO_PRESENT_QUEUE_FAMILY:
            return "Error: unable to find the queue family for presentation!";
        case NO_SURFACE_FORMATS:
            return "Unable to find any surface formats!";
        case NO_PRESENT_MODES:
            return "Unable to find any present modes!";
        default:
            return "Error: unknown error!";
    }
}

void Renderer::loadPlot(const std::string& tPath) {
    std::ifstream File(tPath);
    std::string Line;

    while (std::getline(File, Line)) {
        std::stringstream Stream(Line);
        double Val;

        mData.emplace_back(std::vector <double>());

        while (Stream >> Val) {
            mData.back().emplace_back(Val);
        }
    }

    if (mData.empty()) {
        std::cout << "ALARM!" << std::endl;
    }

    File.close();
}
void Renderer::fillGrid(uint16_t tSizeX, uint16_t tSizeY) {
    Vertices.resize(tSizeX * tSizeY);

    mRed    = 10500 - 500.0 / 3.0;
    mYellow = 10500 - 1000.0 / 3.0;
    mGreen  = 10500 - 500;
    mCyan   = 9500 + 1000.0 / 3.0;
    mBlue   = 9500 + 500.0 / 3.0;

    for (uint16_t i = 0; i < tSizeX; i++) {
        for (uint16_t j = 0; j < tSizeY; j++) {
            Vertices[j * tSizeX + i].pos[0] = -1 + float(i) / float(tSizeX - 1) * 2;
            Vertices[j * tSizeX + i].pos[1] = -1 + float(j) / float(tSizeY - 1) * 2;
            
            if (mData[tSizeY - j - 1][i] <= mDarkBlue) {
                Vertices[j * tSizeX + i].color[0] = 0.0f;
                Vertices[j * tSizeX + i].color[1] = 0.0f;
                Vertices[j * tSizeX + i].color[2] = 0.2f;
            } else if (mData[tSizeY - j - 1][i] <= mBlue && mData[tSizeY - j - 1][i] > mDarkBlue) {
                Vertices[j * tSizeX + i].color[0] = 0.0f;
                Vertices[j * tSizeX + i].color[1] = 0.0f;
                Vertices[j * tSizeX + i].color[2] = 0.2f + float((mData[tSizeY - j - 1][i] - mDarkBlue) / (mBlue - mDarkBlue) * 0.8);
            } else if (mData[tSizeY - j - 1][i] <= mCyan && mData[tSizeY - j - 1][i] > mBlue) {
                Vertices[j * tSizeX + i].color[0] = 0.0f;
                Vertices[j * tSizeX + i].color[1] = float((mData[tSizeY - j - 1][i] - mBlue) / (mCyan - mBlue) * 1.0);
                Vertices[j * tSizeX + i].color[2] = 1.0f;
            } else if (mData[tSizeY - j - 1][i] <= mGreen && mData[tSizeY - j - 1][i] > mCyan) {
                Vertices[j * tSizeX + i].color[0] = 0.0f;
                Vertices[j * tSizeX + i].color[1] = 1.0f;
                Vertices[j * tSizeX + i].color[2] = 1.0f - float((mData[tSizeY - j - 1][i] - mCyan) / (mGreen - mCyan) * 1.0);
            } else if (mData[tSizeY - j - 1][i] <= mYellow && mData[tSizeY - j - 1][i] > mGreen) {
                Vertices[j * tSizeX + i].color[0] = float((mData[tSizeY - j - 1][i] - mGreen) / (mYellow - mGreen) * 1.0);
                Vertices[j * tSizeX + i].color[1] = 1.0f;
                Vertices[j * tSizeX + i].color[2] = 0.0f;
            } else if (mData[tSizeY - j - 1][i] <= mRed && mData[tSizeY - j - 1][i] > mYellow) {
                Vertices[j * tSizeX + i].color[0] = 1.0f;
                Vertices[j * tSizeX + i].color[1] = 1.0f - float((mData[tSizeY - j - 1][i] - mYellow) / (mRed - mYellow) * 1.0);
                Vertices[j * tSizeX + i].color[2] = 0.0f;
            } else if (mData[tSizeY - j - 1][i] <= mDarkRed && mData[tSizeY - j - 1][i] > mRed) {
                Vertices[j * tSizeX + i].color[0] = 1.0f - float((mData[tSizeY - j - 1][i] - mRed) / (mDarkRed - mRed) * 0.8);
                Vertices[j * tSizeX + i].color[1] = 0.0f;
                Vertices[j * tSizeX + i].color[2] = 0.0f;
            } else if (mData[tSizeY - j - 1][i] > mDarkRed) {
                Vertices[j * tSizeX + i].color[0] = 0.2f;
                Vertices[j * tSizeX + i].color[1] = 0.0f;
                Vertices[j * tSizeX + i].color[2] = 0.0f;
            }
        }
    }

    for (uint16_t i = 0; i < tSizeX - 1; i++) {
        for (uint16_t j = 0; j < tSizeY - 1; j++) {
            Indices.emplace_back(j * tSizeX + i);
            Indices.emplace_back(j * tSizeX + (i + 1));
            Indices.emplace_back((j + 1) * tSizeX + i);
            Indices.emplace_back((j + 1) * tSizeX + i);
            Indices.emplace_back(j * tSizeX + (i + 1));
            Indices.emplace_back((j + 1) * tSizeX + (i + 1));
        }
    }
}
//-----------------------------//
void Renderer::setLayers() {
    if (mDebug) {
        mLayersInstance.emplace_back("VK_LAYER_LUNARG_standard_validation");
    }
}
void Renderer::setExtensions() {
    unsigned int ExtCount = 0;

    SDL_Vulkan_GetInstanceExtensions(mWindow, &ExtCount, nullptr);
    mExtensionsInstance.resize(ExtCount);
    SDL_Vulkan_GetInstanceExtensions(mWindow, &ExtCount, mExtensionsInstance.data());

    if (mDebug) {
        mExtensionsInstance.emplace_back("VK_EXT_debug_utils");
    }

    //----------//

    mExtensionsDevice.emplace_back("VK_KHR_swapchain");
}

void Renderer::initInstance() {
    VkApplicationInfo AppInfo = {};
    VkInstanceCreateInfo InstInfo = {};

    AppInfo.sType                       = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    AppInfo.apiVersion                  = VK_MAKE_VERSION(1, 1, 70);
    AppInfo.pApplicationName            = "TestApp";
    AppInfo.pEngineName                 = "TestEngine";

    InstInfo.sType                      = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    InstInfo.pApplicationInfo           = &AppInfo;
    InstInfo.enabledLayerCount          = mLayersInstance.size();
    InstInfo.ppEnabledLayerNames        = mLayersInstance.data();
    InstInfo.enabledExtensionCount      = mExtensionsInstance.size();
    InstInfo.ppEnabledExtensionNames    = mExtensionsInstance.data();

    auto Result = vkCreateInstance(&InstInfo, nullptr, &mInstance);

    if (Result != VK_SUCCESS) {
        throw std::runtime_error(getError(INSTANCE_CREATION_ERROR));
    }

    if (mDebug) {
        initDebug();
    }
}
void Renderer::initSurface() {
    if (!SDL_Vulkan_CreateSurface(mWindow, mInstance, &mSurface)) {
        throw std::runtime_error(getError(SURFACE_CREATION_ERROR) + " " + SDL_GetError());
    }
}
void Renderer::initGPU() {
    uint32_t GPUCount = 0;
    vkEnumeratePhysicalDevices(mInstance, &GPUCount, nullptr);

    if (!GPUCount) {
        throw std::runtime_error(getError(NO_VULKAN_GPU_ERROR));
    }

    std::vector <VkPhysicalDevice> GPUList(GPUCount);
    vkEnumeratePhysicalDevices(mInstance, &GPUCount, GPUList.data());

    mGPU = GPUList[0];
    vkGetPhysicalDeviceProperties(mGPU, &mPropertiesGPU);
}
void Renderer::setQueueFamilyIndices() {
    uint32_t FamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(mGPU, &FamilyCount, nullptr);

    std::vector <VkQueueFamilyProperties> FamilyPropList(FamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(mGPU, &FamilyCount, FamilyPropList.data());

    VkBool32 GraphicsFound = false;
    VkBool32 PresentFound = false;

    for (uint32_t i = 0; i < FamilyCount; i++) {
        if ((FamilyPropList[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && !GraphicsFound) {
            GraphicsFound = true;
            mGraphicsFamilyIndex = i;
        }

        if (!PresentFound) {
            vkGetPhysicalDeviceSurfaceSupportKHR(mGPU, i, mSurface, &PresentFound);

            if (PresentFound) {
                mPresentFamilyIndex = i;
            }
        }

        if (GraphicsFound && PresentFound) {
            break;
        }
    }

    if (!GraphicsFound) {
        throw std::runtime_error(getError(NO_GRAPHICS_QUEUE_FAMILY));
    }

    if (!PresentFound) {
        throw std::runtime_error(getError(NO_PRESENT_QUEUE_FAMILY));
    }
}
void Renderer::setFormat() {
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            mGPU,
            mSurface,
            &mSurfaceCapabilities
            );

    uint32_t FormatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(
            mGPU,
            mSurface,
            &FormatCount,
            nullptr
            );

    if (!FormatCount) {
        throw std::runtime_error(getError(NO_SURFACE_FORMATS));
    }

    std::vector <VkSurfaceFormatKHR> Formats(FormatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(
            mGPU,
            mSurface,
            &FormatCount,
            Formats.data()
    );

    for (const auto& iFormat : Formats) {
        if (iFormat.format == VK_FORMAT_B8G8R8A8_SRGB && iFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            mSurfaceFormat = iFormat;
            return;
        }
    }

    mSurfaceFormat = Formats[0];
}
void Renderer::setPresentMode() {
    uint32_t ModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(
            mGPU,
            mSurface,
            &ModeCount,
            nullptr
    );

    if (!ModeCount) {
        throw std::runtime_error(getError(NO_PRESENT_MODES));
    }

    std::vector <VkPresentModeKHR> Modes(ModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
            mGPU,
            mSurface,
            &ModeCount,
            Modes.data()
    );

//    for (const auto& iMode : Modes) {
//        if (iMode == VK_PRESENT_MODE_MAILBOX_KHR) {
//            mPresentMode = iMode;
//            return;
//        }
//    }

    mPresentMode = VK_PRESENT_MODE_FIFO_KHR;
}
void Renderer::setSwapExtent() {
    VkExtent2D ActualExtent;

    if (mSurfaceCapabilities.currentExtent.width == UINT32_MAX) {
        int Width;
        int Height;

        SDL_Vulkan_GetDrawableSize(mWindow, &Width, &Height);

        ActualExtent.width = static_cast <uint32_t>(Width);
        ActualExtent.height = static_cast <uint32_t>(Height);

        ActualExtent.width = std::clamp(
                ActualExtent.width,
                mSurfaceCapabilities.minImageExtent.width,
                mSurfaceCapabilities.maxImageExtent.width
        );
        ActualExtent.height = std::clamp(
                ActualExtent.height,
                mSurfaceCapabilities.minImageExtent.height,
                mSurfaceCapabilities.maxImageExtent.height
        );
    } else {
        ActualExtent = mSurfaceCapabilities.currentExtent;
    }

    mSwapchainExtent = ActualExtent;
}
void Renderer::createLogicalDevice() {
    float QueuePriority = 1.0f;

    std::vector <VkDeviceQueueCreateInfo> DevQueuesInfo;
    std::set <uint32_t> UniqueQueueFamilies = {mGraphicsFamilyIndex, mPresentFamilyIndex};
    VkDeviceCreateInfo DevInfo = {};
    VkPhysicalDeviceFeatures DevFeatures = {};

    for (uint32_t iQueueIndex : UniqueQueueFamilies) {
        VkDeviceQueueCreateInfo QueueInfo = {};

        QueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        QueueInfo.queueFamilyIndex = iQueueIndex;
        QueueInfo.queueCount = 1;
        QueueInfo.pQueuePriorities = &QueuePriority;

        DevQueuesInfo.emplace_back(QueueInfo);
    }

    DevInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    DevInfo.queueCreateInfoCount    = static_cast <uint32_t>(DevQueuesInfo.size());
    DevInfo.pQueueCreateInfos       = DevQueuesInfo.data();
    DevInfo.enabledExtensionCount   = static_cast <uint32_t>(mExtensionsDevice.size());
    DevInfo.ppEnabledExtensionNames = mExtensionsDevice.data();
    DevInfo.pEnabledFeatures        = &DevFeatures;

    auto Result = vkCreateDevice(mGPU, &DevInfo, nullptr, &mDevice);

    if (Result != VK_SUCCESS) {
        throw std::runtime_error(getError(LOGICAL_DEVICE_CREATION_ERROR));
    }

    vkGetDeviceQueue(mDevice, mGraphicsFamilyIndex, 0, &mGraphicsQueue);
    vkGetDeviceQueue(mDevice, mPresentFamilyIndex, 0, &mPresentQueue);
}
void Renderer::createSwapchain() {
    setFormat();
    setPresentMode();
    setSwapExtent();

    mSwapChainImageCount = mSurfaceCapabilities.minImageCount + 1;

    if (mSurfaceCapabilities.maxImageCount > 0 && mSwapChainImageCount > mSurfaceCapabilities.maxImageCount) {
        mSwapChainImageCount = mSurfaceCapabilities.maxImageCount;
    }

    uint32_t QueueFamilyIndices[] = {mGraphicsFamilyIndex, mPresentFamilyIndex};

    VkSwapchainCreateInfoKHR SwapchainInfo = {};

    SwapchainInfo.sType						= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    SwapchainInfo.surface					= mSurface;
    SwapchainInfo.minImageCount				= mSwapChainImageCount;
    SwapchainInfo.imageFormat				= mSurfaceFormat.format;
    SwapchainInfo.imageColorSpace			= mSurfaceFormat.colorSpace;
    SwapchainInfo.imageExtent			    = mSwapchainExtent;
    SwapchainInfo.imageArrayLayers			= 1;
    SwapchainInfo.imageUsage				= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (mGraphicsFamilyIndex != mPresentFamilyIndex) {
        SwapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        SwapchainInfo.queueFamilyIndexCount = 2;
        SwapchainInfo.pQueueFamilyIndices = QueueFamilyIndices;
    } else {
        SwapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        SwapchainInfo.queueFamilyIndexCount = 0;
        SwapchainInfo.pQueueFamilyIndices = nullptr;
    }

    SwapchainInfo.preTransform				= VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    SwapchainInfo.compositeAlpha			= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    SwapchainInfo.presentMode				= mPresentMode;
    SwapchainInfo.clipped					= VK_TRUE;
    SwapchainInfo.oldSwapchain				= VK_NULL_HANDLE;

    auto Result = vkCreateSwapchainKHR(mDevice, &SwapchainInfo, nullptr, &mSwapChain);

    if (Result != VK_SUCCESS) {
        throw std::runtime_error(getError(SWAP_CHAIN_CREATION_ERROR));
    }

    vkGetSwapchainImagesKHR(mDevice, mSwapChain, &mSwapChainImageCount, nullptr);
    mSwapchainImages.resize(mSwapChainImageCount);
}
void Renderer::initSwapchainImages() {
    mSwapchainImageViews.resize(mSwapChainImageCount);

    vkGetSwapchainImagesKHR(
            mDevice,
            mSwapChain,
            &mSwapChainImageCount,
            mSwapchainImages.data()
            );

    for (uint32_t i = 0; i < mSwapChainImageCount; i++) {
        VkImageViewCreateInfo ImageViewInfo = {};

        ImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        ImageViewInfo.image = mSwapchainImages[i];
        ImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        ImageViewInfo.format = mSurfaceFormat.format;
        ImageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        ImageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        ImageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        ImageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        ImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        ImageViewInfo.subresourceRange.baseMipLevel = 0;
        ImageViewInfo.subresourceRange.levelCount = 1;
        ImageViewInfo.subresourceRange.baseArrayLayer = 0;
        ImageViewInfo.subresourceRange.layerCount = 1;

        auto Result = vkCreateImageView(
                mDevice,
                &ImageViewInfo,
                nullptr,
                &mSwapchainImageViews[i]
        );

        if (Result != VK_SUCCESS) {
            throw std::runtime_error(getError(IMAGE_VIEW_CREATION_ERROR));
        }
    }
}
void Renderer::initRenderPass() {
    VkAttachmentDescription ColorAttachment = {};

    ColorAttachment.format = mSurfaceFormat.format;
    ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    ColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    ColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference ColorAttachmentRef = {};

    ColorAttachmentRef.attachment = 0;
    ColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription Subpass = {};

    Subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    Subpass.colorAttachmentCount = 1;
    Subpass.pColorAttachments = &ColorAttachmentRef;

    VkSubpassDependency Dependency = {};
    Dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    Dependency.dstSubpass = 0;
    Dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    Dependency.srcAccessMask = 0;
    Dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    Dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo RenderPassInfo = {};

    RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    RenderPassInfo.attachmentCount = 1;
    RenderPassInfo.pAttachments = &ColorAttachment;
    RenderPassInfo.subpassCount = 1;
    RenderPassInfo.pSubpasses = &Subpass;
    RenderPassInfo.dependencyCount = 1;
    RenderPassInfo.pDependencies = &Dependency;

    if (vkCreateRenderPass(mDevice, &RenderPassInfo, nullptr, &mRenderPass) != VK_SUCCESS) {
        std::cout << "Failed to create the render pass!" << std::endl;
        exit(-1);
    }
}
void Renderer::initGraphicsPipeline() {
    auto VertShaderCode = readFile("Vert.spv");
    auto FragShaderCode = readFile("Frag.spv");

    auto VertShaderModule = createShaderModule(VertShaderCode);
    auto FragShaderModule = createShaderModule(FragShaderCode);

    VkPipelineShaderStageCreateInfo VertShaderInfo = {};
    VkPipelineShaderStageCreateInfo FragShaderInfo = {};

    VertShaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    VertShaderInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    VertShaderInfo.module = VertShaderModule;
    VertShaderInfo.pName = "main";

    FragShaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    FragShaderInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    FragShaderInfo.module = FragShaderModule;
    FragShaderInfo.pName = "main";

    VkPipelineShaderStageCreateInfo ShaderStages[] = {VertShaderInfo, FragShaderInfo};

    auto BindingDescription = Vertex::getBindingDescription();
    auto AttributeDescriptions = Vertex::getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo VertexInputInfo = {};

    VertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    VertexInputInfo.vertexBindingDescriptionCount = 1;
    VertexInputInfo.pVertexBindingDescriptions = &BindingDescription;
    VertexInputInfo.vertexAttributeDescriptionCount = static_cast <uint32_t>(AttributeDescriptions.size());
    VertexInputInfo.pVertexAttributeDescriptions = AttributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo InputAssembly = {};

    InputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    InputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    InputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport Viewport = {};

    Viewport.x = 0.0f;
    Viewport.y = 0.0f;
    Viewport.width = (float)mSwapchainExtent.width;
    Viewport.height = (float)mSwapchainExtent.height;
    Viewport.minDepth = 0.0f;
    Viewport.maxDepth = 1.0f;

    VkRect2D Scissor = {};

    Scissor.offset = {0, 0};
    Scissor.extent = mSwapchainExtent;

    VkPipelineViewportStateCreateInfo ViewportState = {};

    ViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    ViewportState.viewportCount = 1;
    ViewportState.pViewports = &Viewport;
    ViewportState.scissorCount = 1;
    ViewportState.pScissors = &Scissor;

    VkPipelineRasterizationStateCreateInfo Rasterizer = {};

    Rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    Rasterizer.depthBiasEnable = VK_FALSE;
    Rasterizer.rasterizerDiscardEnable = VK_FALSE;
    Rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    Rasterizer.lineWidth = 1.0f;
    Rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    Rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    Rasterizer.depthBiasEnable = VK_FALSE;
    Rasterizer.depthBiasConstantFactor = 0.0f;
    Rasterizer.depthBiasClamp = 0.0f;
    Rasterizer.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo Multisampling = {};

    Multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    Multisampling.sampleShadingEnable = VK_FALSE;
    Multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    Multisampling.minSampleShading = 1.0f;
    Multisampling.pSampleMask = nullptr;
    Multisampling.alphaToCoverageEnable = VK_FALSE;
    Multisampling.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState ColorBlendAttachment = {};

    ColorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT |
            VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
    ColorBlendAttachment.blendEnable = VK_FALSE;
    ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    ColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    ColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo ColorBlending = {};

    ColorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    ColorBlending.logicOpEnable = VK_FALSE;
    ColorBlending.logicOp = VK_LOGIC_OP_COPY;
    ColorBlending.attachmentCount = 1;
    ColorBlending.pAttachments = &ColorBlendAttachment;
    ColorBlending.blendConstants[0] = 0.0f;
    ColorBlending.blendConstants[1] = 0.0f;
    ColorBlending.blendConstants[2] = 0.0f;
    ColorBlending.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo PipelineLayoutInfo = {};

    PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    PipelineLayoutInfo.setLayoutCount = 0;
    PipelineLayoutInfo.pSetLayouts = nullptr;
    PipelineLayoutInfo.pushConstantRangeCount = 0;
    PipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(mDevice, &PipelineLayoutInfo, nullptr, &mLayout) != VK_SUCCESS) {
        std::cout << "Failed to create pipeline layout!" << std::endl;
        exit(-1);
    }

    VkGraphicsPipelineCreateInfo PipelineInfo = {};

    PipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    PipelineInfo.stageCount = 2;
    PipelineInfo.pStages = ShaderStages;
    PipelineInfo.pVertexInputState = &VertexInputInfo;
    PipelineInfo.pInputAssemblyState = &InputAssembly;
    PipelineInfo.pViewportState = &ViewportState;
    PipelineInfo.pRasterizationState = &Rasterizer;
    PipelineInfo.pMultisampleState = &Multisampling;
    PipelineInfo.pDepthStencilState = nullptr;
    PipelineInfo.pColorBlendState = &ColorBlending;
    PipelineInfo.pDynamicState = nullptr;
    PipelineInfo.layout = mLayout;
    PipelineInfo.renderPass = mRenderPass;
    PipelineInfo.subpass = 0;
    PipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    PipelineInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(
            mDevice,
            VK_NULL_HANDLE,
            1, &PipelineInfo,
            nullptr,
            &mPipeline) != VK_SUCCESS) {
        std::cout << "Failed to create graphics pipeline!" << std::endl;
        exit(-1);
    }

    vkDestroyShaderModule(mDevice, FragShaderModule, nullptr);
    vkDestroyShaderModule(mDevice, VertShaderModule, nullptr);
}
void Renderer::initFrameBuffers() {
    mSwapchainFramebuffers.resize(mSwapchainImageViews.size());

    for (size_t i = 0; i < mSwapchainImageViews.size(); i++) {
        VkImageView Attachments[] = {mSwapchainImageViews[i]};

        VkFramebufferCreateInfo FrameBufferInfo = {};

        FrameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        FrameBufferInfo.renderPass = mRenderPass;
        FrameBufferInfo.attachmentCount = 1;
        FrameBufferInfo.pAttachments = Attachments;
        FrameBufferInfo.width = mSwapchainExtent.width;
        FrameBufferInfo.height = mSwapchainExtent.height;
        FrameBufferInfo.layers = 1;

        if (vkCreateFramebuffer(mDevice, &FrameBufferInfo, nullptr, &mSwapchainFramebuffers[i]) != VK_SUCCESS) {
            std::cout << "Failed to create a frame buffer!" << std::endl;
            exit(-1);
        }
    }
}
void Renderer::initCommandPool() {
    VkCommandPoolCreateInfo PoolInfo = {};

    PoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    PoolInfo.queueFamilyIndex = mGraphicsFamilyIndex;
    PoolInfo.flags = 0;

    if (vkCreateCommandPool(mDevice, &PoolInfo, nullptr, &mCommandPool) != VK_SUCCESS) {
        std::cout << "Failed to create a command pool!" << std::endl;
        exit(-1);
    }
}
void Renderer::initVertexBuffer() {
    VkDeviceSize BufferSize = sizeof(float) * 5 * Vertices.size();

//    VkBuffer StagingBuffer;
//    VkDeviceMemory StagingBufferMemory;
//
//    createBuffer(
//            BufferSize,
//            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
//            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//            StagingBuffer,
//            StagingBufferMemory);
//
//    void* Data;
//    vkMapMemory(mDevice, StagingBufferMemory, 0, BufferSize, 0, &Data);
//    {
//        memcpy(Data, Vertices.data(), size_t(BufferSize));
//    }
//    vkUnmapMemory(mDevice, StagingBufferMemory);

//
//    createBuffer(
//            BufferSize,
//            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
//            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
//            mVertexBuffer,
//            mVertexBufferMemory);

//    copyBuffer(StagingBuffer, mVertexBuffer, BufferSize);

//    vkDestroyBuffer(mDevice, StagingBuffer, nullptr);
//    vkFreeMemory(mDevice, StagingBufferMemory, nullptr);

    createBuffer(
            BufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            mVertexBuffer,
            mVertexBufferMemory);

    void* Data;
    vkMapMemory(mDevice, mVertexBufferMemory, 0, BufferSize, 0, &Data);
    {
        memcpy(Data, Vertices.data(), size_t(BufferSize));
    }
    vkUnmapMemory(mDevice, mVertexBufferMemory);
}
void Renderer::initIndexBuffer() {
    VkDeviceSize bufferSize = sizeof(Indices[0]) * Indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory);

    void* data;
    vkMapMemory(mDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, Indices.data(), (size_t) bufferSize);
    vkUnmapMemory(mDevice, stagingBufferMemory);

    createBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            mIndexBuffer,
            mIndexBufferMemory);

    copyBuffer(stagingBuffer, mIndexBuffer, bufferSize);

    vkDestroyBuffer(mDevice, stagingBuffer, nullptr);
    vkFreeMemory(mDevice, stagingBufferMemory, nullptr);
}
void Renderer::initCommandBuffers() {
    mCommandBuffers.resize(mSwapchainFramebuffers.size());

    VkCommandBufferAllocateInfo AllocInfo = {};

    AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    AllocInfo.commandPool = mCommandPool;
    AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    AllocInfo.commandBufferCount = (uint32_t)mCommandBuffers.size();

    if (vkAllocateCommandBuffers(mDevice, &AllocInfo, mCommandBuffers.data()) != VK_SUCCESS) {
        std::cout << "Failed to allocate command buffers!" << std::endl;
        exit(-1);
    }

    for (size_t i = 0; i < mCommandBuffers.size(); i++) {
        VkCommandBufferBeginInfo BeginInfo = {};

        BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        BeginInfo.flags = 0;
        BeginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(mCommandBuffers[i], &BeginInfo) != VK_SUCCESS) {
            std::cout << "Failed to begin recording command buffer!" << std::endl;
            exit(-1);
        }

        VkClearValue ClearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        VkRenderPassBeginInfo RenderPassInfo = {};

        RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        RenderPassInfo.renderPass = mRenderPass;
        RenderPassInfo.framebuffer = mSwapchainFramebuffers[i];
        RenderPassInfo.renderArea.offset = {0, 0};
        RenderPassInfo.renderArea.extent = mSwapchainExtent;
        RenderPassInfo.clearValueCount = 1;
        RenderPassInfo.pClearValues = &ClearColor;

        vkCmdBeginRenderPass(mCommandBuffers[i], &RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(mCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline);

        VkBuffer vertexBuffers[] = {mVertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(mCommandBuffers[i], 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(mCommandBuffers[i], mIndexBuffer, 0, VK_INDEX_TYPE_UINT16);

        vkCmdDrawIndexed(mCommandBuffers[i], static_cast<uint32_t>(Indices.size()), 1, 0, 0, 0);

        vkCmdEndRenderPass(mCommandBuffers[i]);

        if (vkEndCommandBuffer(mCommandBuffers[i]) != VK_SUCCESS) {
            std::cout << "Failed to record command buffer!" << std::endl;
            exit(-1);
        }
    }
}
void Renderer::initSyncObjects() {
    mImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    mRenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    mFences.resize(MAX_FRAMES_IN_FLIGHT);
    mImagesInFlight.resize(mSwapchainImages.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo SemaphoreInfo = {};
    SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo FenceInfo = {};
    FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    FenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (
                vkCreateSemaphore(mDevice, &SemaphoreInfo, nullptr, &mImageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(mDevice, &SemaphoreInfo, nullptr, &mRenderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(mDevice, &FenceInfo, nullptr, &mFences[i]) != VK_SUCCESS
                ) {
            std::cout << "Failed to create semaphore!" << std::endl;
            exit(-1);
        }
    }
}
//-----------------------------//
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT          tMsgSeverity,
        VkDebugUtilsMessageTypeFlagsEXT                 tMsgType,
        const VkDebugUtilsMessengerCallbackDataEXT*     tCallbackData,
        void*                                           tUserData
    ) {
    if (tMsgSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
        std::cout << "VERBOSE: ";
    }
    if (tMsgSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
        std::cout << "INFO: ";
    }
    if (tMsgSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        std::cout << "WARNING: ";
    }
    if (tMsgSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        std::cout << "ERROR: ";
    }

    //----------//

    if (tMsgType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
        std::cout << "GENERAL: ";
    } else {
        if (tMsgType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
            std::cout << "VALIDATION: ";
        }
        if (tMsgType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
            std::cout << "SPEC: ";
        }
    }

    std::cout << tCallbackData -> pMessage << std::endl;

    return false;
}
PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT      = VK_NULL_HANDLE;
PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessengerEXT    = VK_NULL_HANDLE;

void Renderer::initDebug() {
    mDebugInfo.sType            = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    mDebugInfo.messageSeverity  =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    mDebugInfo.messageType      =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT  |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    mDebugInfo.pfnUserCallback  = debugCallback;

    CreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            mInstance,
            "vkCreateDebugUtilsMessengerEXT");
    DestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            mInstance,
            "vkDestroyDebugUtilsMessengerEXT");

    if (!CreateDebugUtilsMessengerEXT || !DestroyDebugUtilsMessengerEXT) {
        throw std::runtime_error(getError(DEBUG_UTILS_EXTENSION_ERROR));
    }

    auto Result = CreateDebugUtilsMessengerEXT(mInstance, &mDebugInfo, nullptr, &mDebugReport);

    if (Result != VK_SUCCESS) {
        throw std::runtime_error(getError(DEBUG_UTILS_MESSENGER_ERROR));
    }
}
void Renderer::destroyDebug() {
    DestroyDebugUtilsMessengerEXT(mInstance, mDebugReport, nullptr);
    mDebugReport = VK_NULL_HANDLE;
}